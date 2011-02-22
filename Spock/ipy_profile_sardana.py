"""Settings for Sardana session SARDANA"""

import IPython

def main():
    ip = IPython.ipapi.get()
    print "Setting %s environment" % ip.user_ns['PROFILE']
    ip.user_ns['MACRO_SERVER'] = 'pc077/macroserver/1'
    ip.user_ns['DOOR'] = ''
    
main()
