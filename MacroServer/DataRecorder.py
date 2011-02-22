
import time, sps, numpy, os

class DataFormats:
   SPEC  = 1
   CSV   = 2
   EXCEL = 3

class SaveModes: 
   RECORD =  1
   BLOCK  =  2

class RecorderStatus: 
   IDLE    =  1
   ACTIVE  =  2
   DISABLE =  3

class DataType:
   INT     = 1
   FLOAT   = 2
   FILEREF = 3

class DataHandler:
   """ The data handler is the data recording center of a system. It contains
   one or several recorders.  All data transit through the handler, then 
   given to recorders for final saving """


   def __init__(self):
       self.recorders = []
       self.history   = []
    
       self.serialno = 0
   
   def addRecorder(self, recorder):
       self.recorders.append( recorder )
   
   def startRecordList(self, recordlist ):
       recordlist.setEnvironValue('starttime', time.time() )
   
       for recorder in self.recorders:
          if recorder.savemode == SaveModes.RECORD:
             recorder.startRecordList( recordlist )
   
   def getSerialNo(self):
       self.serialno += 1
       return self.serialno

   def endRecordList(self, recordlist):
   
       recordlist.setEnvironValue('endtime', time.time() )
   
       for recorder in self.recorders:
           if recorder.savemode == SaveModes.RECORD:
               recorder.endRecordList( recordlist )
           else:
               recorder.writeRecordList( recordlist )
   
   def addRecord(self, recordlist, record):
   
       for recorder in self.recorders:
           if recorder.savemode == SaveModes.RECORD:
               recorder.writeRecord( record )
           else:    # blockSave
               pass
           
  #
  # Recorders
  #

class DataRecorder:
    """ Generic class for data recorder. Does nothing"""
    def __init__(self, **pars):
        self.recordlist = None
        self.status     = RecorderStatus.IDLE
        self.savemode   = SaveModes.RECORD

    def getStatus(self):
        return self.status

    def disable(self):
        self.status = RecorderStatus.DISABLE

    def enable(self):
        self.status = RecorderStatus.IDLE

    def startRecordList(self, recordlist):

        self._startRecordList(recordlist)

        if self.status == RecorderStatus.IDLE:
           self.recordlist = recordlist
           return 0
        else:
           return -1

    def _startRecordList(self, recordlist):
        pass

    def endRecordList(self, recordlist):
        self._endRecordList(recordlist)

        self.status     = RecorderStatus.IDLE
        self.recordlist = None

    def _endRecordList(self, recordlist):
        pass

    def writeRecordList(self, recordlist):
        """ Only in BLOCK_MODE. Will write whold RecordList """
        self._startRecordList( recordlist )
        self._endRecordList( recordlist )
        for record in recordlist.records:
           self.writeRecord(record)

    def writeRecord(self, record):
        self._writeRecord( record )

    def _writeRecord( self, record ):
        pass

    def setSaveMode( self, mode ):
        self.savemode = mode

    def env_changed(self,prop_name,prop_value):
        pass
        

class DumbRecorder(DataRecorder):
    def _startRecordList(self, recordlist):
        print "Starting new recording"
        print "# Title :     ", recordlist.getEnvironValue('title')
        env = recordlist.getEnviron()
        for envky in env.keys():
           if envky != 'title' and envky != 'labels':
               print "# %8s :    %s " % (envky,str(env[envky]))
        print "# Started:    ", time.ctime( env['starttime'] )
        print "# L:  ",   
        print "  ".join( env['labels'] )

    def _writeRecord(self, record):
        print record.data

    def _endRecordList(self, recordlist):
        print "Ending recording"
        env = recordlist.getEnviron()
        print "Recording ended at: ", time.ctime( env['endtime'] )

    def env_changed(self,prop_name,prop_value):
        pass

class FileRecorder(DataRecorder):
    """ Saves data to a file """

    def __init__(self, filename=None, format=DataFormats.SPEC, **pars):
        self.filename = filename
        self.format   = format
        self.fd       = None 
        if self.filename:
             self.setFileName( filename, format)
        DataRecorder.__init__(self)

    def setFileName(self, filename, format=DataFormats.SPEC):

        if self.fd != None:
           self.fd.close()
   
        self.filename    = filename
        self.format      = format
        self.currentlist = None

    def _startRecordList(self, recordlist):

        if self.filename == None:
              return

        self.fd          = open(self.filename,'a')

        self.currentlist = recordlist

        print "[FileRecorder] starting new recording %d on file %s" % (recordlist.serialno, self.filename)

        env  = recordlist.getEnviron()
        data = {
                'serialno':  recordlist.serialno,
                'title':     env['title'],
                'user':      os.getenv('user'),
                'epoch':     env['starttime'],
                'starttime': time.ctime( env['starttime'] ),
                'nocols':    len(env['labels']),
                'labels':    "  ".join( env['labels'] )
               }
        self.fd.write("""
#S %(serialno)s %(title)s
#U %(user)s
#D %(epoch)s
#C Acquisition started at %(starttime)s
#N %(nocols)s
#L %(labels)s
""" % data )
        self.fd.flush()

    def _writeRecord(self, record):
        if self.filename == None:
              return
        outstr = ""
        labels = self.currentlist.getEnvironValue('labels')

        for colname in labels:
            if record.data.has_key( colname ):
               outstr += str( record.data[colname] )
            else:
               print "missing column in data ", colname
               outstr += "NaN" 
            outstr += ' '

        self.fd.write( outstr + '\n')
        self.fd.flush()

    def _endRecordList(self, recordlist):

        if self.filename == None:
              return

        env = recordlist.getEnviron()
        self.fd.write("#C Acquisition ended at %s\n" % time.ctime( env['endtime'] ))
        self.fd.flush()
        self.fd.close()
        self.currentlist = None

    def env_changed(self,prop_name,prop_value):
        if prop_name.lower() == 'DataFile'.lower():
            self.setFileName(prop_value)

class ShmRecorder(DataRecorder):

    """ Sets data in shared memory to be used by sps """

    maxenv = 50
    envlen = 1024

    def setShmID(self, shmid):
        self.shm_id = shmid
        self.shm_id_env = shmid + "_ENV"

    def setProgram(self, progname):
        self.progname = progname

    def setSize(self, rows, cols):
        self.rows = rows
        self.cols = cols

    def putenv(self, name, value):
        print "[SHM] putenv",name, value
        sps.putenv( self.progname, self.shm_id_env, name, str(value) )

    def _startRecordList(self, recordlist):

        print "[SHM] start on",self.progname,self.shm_id

        arraylist = sps.getarraylist( self.progname )

        if self.shm_id in arraylist:
            shm = sps.attach( self.progname, self.shm_id )
        else:
            sps.create( self.progname, self.shm_id, self.rows, self.cols, sps.DOUBLE )

        if self.shm_id_env in arraylist:
            shm_env = sps.attach( self.progname, self.shm_id_env )
        else:
            sps.create( self.progname, self.shm_id_env, self.maxenv, self.envlen, sps.STRING )

        print "Starting new SHM recording"

        self.putenv( 'title', recordlist.getEnvironValue('title') )

        for env,val in recordlist.getEnviron().items():
           if env != 'title' and env != 'labels':
               self.putenv( env , val)

        self.putenv('started', time.ctime( recordlist.getEnvironValue('starttime') ))
        self.putenv('ended', '')
        self.putenv('axistitles', ' '.join( recordlist.getEnvironValue('labels') ))

    def _writeRecord(self, record):
        print "[SHM] writerecord"
        # uhmm. only numeric values can be written
        #sps.putdatarow( self.progname, self.shm_id, record.recordno, record.data )
        vals =  [ val for val in record.data.values() if type(val) in [int,float,long] ]
        vals = numpy.array( vals )
        sps.putdatarow( self.progname, self.shm_id, record.recordno, vals )

    def _endRecordList(self, recordlist):
        print "[SHM] end"
        self.putenv('ended', time.ctime( recordlist.getEnvironValue('endtime') ))

    def env_changed(self,prop_name,prop_value):
        if prop_name.lower() == 'ShmProgram'.lower():
            self.setProgram(prop_value)
        elif prop_name.lower() == 'ShmID'.lower():
            self.setShmID(prop_value)

class LoggerRecorder(DataRecorder):
    """ It may be used to display data on the terminal using the Logger features"""
    pass

class DatabaseRecorder(DataRecorder):
    """ Saves data to a database. Not implemented for now. Could be interesting
    to save some logging information
    """
    pass



