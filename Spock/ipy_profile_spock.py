"""Settings for Spock session"""

import IPython

def main():
    ip = IPython.ipapi.get()
    print "Setting %s environment" % ip.user_ns['PROFILE']
    
    ip.user_ns['MACRO_SERVER'] = '@macroserver_name@'
    ip.user_ns['DOOR'] = '@door_name@'
        
main()
