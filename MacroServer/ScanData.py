
from DataRecorder import DataHandler

class DataType:
   INT     = 1
   FLOAT   = 2
   FILEREF = 3

  #
  # Data classes
  #
class RecordEnvironment(dict):
    """  A RecordEnvironment is a set of arbitrary pairs of type 
    label/value in the form of a dictionary.
    """
    __needed = ['title', 'labels']

    def isValid(self):
        """ Check valid environment = all needed keys present """

        if not needed:  return 1

        for ky in self.needed + self.__needed:
           if ky not in self.keys():
              return 0 
        else:
           return 1

class ScanDataEnvironment(RecordEnvironment):
    """
    It describes a recordlist and its environment

    A recordlist environment contains a number of predefined label/value pairs
    Values can be either a string, a numeric value or a list of strings, numbers
      title:     mandatory
      labels:    mandatory. label for each of the fields in a record of the recordlist
      fielddesc: description of the content of each of the fields in a record. Can be used
          to affect the way the field is saved by the recorder. If not present all fields 
          are by default of type FLOAT and FORMAT ".8g"
    """
    needed = ['title', 'labels']
  
class RecordList(dict):
    """  A RecordList is a set of records: for example a scan.
    It is composed of a environment and a list of records
 """
    def __init__(self, datahandler, environ=None):

        self.datahandler = datahandler
        if environ == None:
           self.environ = RecordEnvironment()
        else:
           self.environ = environ
        self.records     = []
        self.serialno    = self.datahandler.getSerialNo()

    def setEnviron( self, environ):
        self.environ = environ

    def updateEnviron( self, environ):
        self.environ.update( environ )

    def setEnvironValue(self, name, value):
        self.environ[name] = value

    def getEnvironValue(self, name):
        return self.environ[name]

    def getEnviron(self):
        return self.environ

    def start(self):
        self.recordno = 0
        self.datahandler.startRecordList( self )
 
    def addRecord(self, record):
        rc = Record( record )
        rc.setRecordNo( self.recordno )
        self.records.append( rc )
        print self.recordno, " record added"
        self.recordno  += 1
        
        self.datahandler.addRecord( self, rc )

    def addRecords(self, records):
        map( self.addRecord, records )

    def end(self):
        self.datahandler.endRecordList( self )

class ScanData(RecordList):
    def __init__(self, environment=None,data_handler=None):
        dh = data_handler or DataHandler()
        RecordList.__init__(self, dh, environment)

class Record:
    """ One record is a set of values measured at the same time """

    def __init__(self, datalist):
        self.recordno = 0
        self.data     = datalist
        self.complete = 0
        self.written  = 0

    def setRecordNo (self, recordno):
        self.recordno = recordno

    def setComplete(self):
        self.complete = 1

    def setWritten(self):
        self.written = 1

def main():
   
   from DataRecorder import DataHandler
   from DataRecorder import FileRecorder,DumbRecorder,LoggerRecorder,ShmRecorder
   from DataRecorder import DataFormats,SaveModes
   #-----------------------------------------------------------------------------------
   #
   # Datahandler and recorders should be configured by the system
   #
   dh    = DataHandler()

   filer = FileRecorder( filename="/tmp/test.dat", format=DataFormats.CSV )
   filer.setSaveMode( SaveModes.BLOCK )

   dumbr = DumbRecorder( )
   dumbr.setSaveMode( SaveModes.BLOCK )

   termr = LoggerRecorder( )
   termr.setSaveMode( SaveModes.RECORD )

   shmr = ShmRecorder( )
   shmr.setSaveMode( SaveModes.RECORD )
   shmr.setSize( 1024, 3 )
   shmr.setProgram( "vicente" )
   shmr.setShmID( "testshm" )

   #dh.addRecorder( filer )
   dh.addRecorder( dumbr )
   dh.addRecorder( shmr )

   #----------------------------------------------------------------------------
   #
   # save some fake data as it may appear in a scan
   #

   env = ScanDataEnvironment ()

   env['title']        = 'Aunty Watch - April'
   env['description']  = 'Weight Watcher'
   env['myaunty']      = 'Margaret'
   env['filebase']     = '/home/vrey/pics'
   env['labels']       = [ 'Day',        'Weight',         'AuntyPic' ] 
   env['datadesc']     = [ DataType.INT,  DataType.FLOAT,  DataType.FILEREF ]

   # an alternative way would be to get the datahandler (unique) 
   # from the system

   sd = ScanData( env )
   sd.start()

    # one by one
   d1 = {'Day':  121,  'Weight': 73.4,  'AuntyPic':  'pics121.jpg' }
   sd.addRecord( d1 )
   d2 = {'Day':  122,  'Weight': 73.3,  'AuntyPic':  'pics122.jpg' }
   sd.addRecord( d2 )
   d3 = {'Day':  123,  'Weight': 73.2,  'AuntyPic':  'pics123.jpg' }
   sd.addRecord( d3 )

    # or several at one time
   d4 = {'Day':  124,  'Weight': 73.2,  'AuntyPic':  'pics124.jpg' }
   d5 = {'Day':  125,  'Weight': 73.6,  'AuntyPic':  'pics125.jpg' }
   d6 = {'Day':  126,  'Weight': 74.0,  'AuntyPic':  'pics126.jpg' }
   d7 = {'Day':  127,  'Weight': 73.5,  'AuntyPic':  'pics127.jpg' }

   sd.addRecords( [ d4, d5, d6, d7 ] )

   sd.end()
   
if __name__ == '__main__':
   import time

   main()
   time.sleep(20) 
