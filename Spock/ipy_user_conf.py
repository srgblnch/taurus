""" User configuration file for IPython

This is a more flexible and safe way to configure ipython than *rc files
(ipythonrc, ipythonrc-pysh etc.)

This file is always imported on ipython startup. You can import the
ipython extensions you need here (see IPython/Extensions directory).

Feel free to edit this file to customize your ipython experience.

Note that as such this file does nothing, for backwards compatibility.
Consult e.g. file 'ipy_profile_sh.py' for an example of the things 
you can do here.

See http://ipython.scipy.org/moin/IpythonExtensionApi for detailed
description on what you could do here.
"""

# Most of your config files and extensions will probably start with this import

import IPython
import IPython.ipapi
import IPython.Release

import sys

__version__ = '0.1'

from spock import imacro

def main():
    print "Setting global environment"
    ip = IPython.ipapi.get()

    # Handy tab-completers for %cd, %run, import etc.
    # Try commenting this out if you have completion problems/slowness
    import ipy_stock_completers

    pyver = sys.version.split(' ')[0]
    ipyver = IPython.Release.version
    spockver = __version__
    
    # IPython options
    o = ip.options
    o.autocall = 1
    o.autoedit_syntax = 0
    o.autoindent = 1
    o.automagic = 1
    o.cache_size = 1000
    o.colors = 'Linux'
    o.color_info = 1
    o.confirm_exit = 0
    o.deep_reload = 0
    o.editor = 'vi'
    o.log = 0
    o.logfile = ''
    o.messages = 1
    o.pdb = 0
    o.pprint = 1
    #o.prompt_in1 = 'In [\#]: '
    #o.prompt_out = 'Out[\#]: '
    o.quick = 0
    o.readline = 1
    o.screen_length = 0
    o.separate_in = '\n'
    o.separate_out = '\n'
    o.separate_out2 = ''
    o.nosep = 0
    o.wildcards_case_sensitive = 1
    o.object_info_string_level = 0
    o.xmode = 'Context'
    o.multi_line_specials = 1
    o.system_header = "IPython system call: "
    o.system_verbose = 1
    o.wxversion = '0'
    o.readline_parse_and_bind.append('tab: complete')
    #o.readline_parse_and_bind.append('tab: menu-complete')
    o.readline_parse_and_bind.append('"\C-l": possible-completions')
    o.readline_parse_and_bind.append('set show-all-if-ambiguous on')
    o.readline_parse_and_bind.append('"\C-o": tab-insert')
    o.readline_parse_and_bind.append('"\M-i": "    "')
    o.readline_parse_and_bind.append('"\M-o": "\d\d\d\d"')
    o.readline_parse_and_bind.append('"\M-I": "\d\d\d\d"')
    o.readline_parse_and_bind.append('"\C-r": reverse-search-history')
    o.readline_parse_and_bind.append('"\C-s": forward-search-history')
    o.readline_parse_and_bind.append('"\C-p": history-search-backward')
    o.readline_parse_and_bind.append('"\C-n": history-search-forward')
    o.readline_parse_and_bind.append('"\e[A": history-search-backward')
    o.readline_parse_and_bind.append('"\e[B": history-search-forward')
    o.readline_parse_and_bind.append('"\C-k": kill-line')
    o.readline_parse_and_bind.append('"\C-u": unix-line-discard')
    o.readline_remove_delims = '-/~'
    o.readline_merge_completions = 1
    o.readline_omit__names = 0

    del ip.IP.alias_table["mv"]

    # Macro server
    profile = ip.user_ns.get('PROFILE')
    macro_server = ip.user_ns.get('MACRO_SERVER')
    door = ip.user_ns.get('DOOR')

    o.banner =  'Spock %s -- An interactive Macro Server client.\n' % spockver
    o.banner += '\nRunning on top of Python %s and IPython %s\n' % (pyver,ipyver)
    
    #o.banner =  'Python %s\n' % pyver
    #o.banner += 'Type "copyright", "credits" or "license" for more information.\n'
    #o.banner += '\nIPython %s -- An enhanced Interactive Python.\n' % ipyver
    #o.banner += "?       -> Introduction to IPython's features.\n"
    #o.banner += """%magic  -> Information about 'magic' % functions.\n"""
    #o.banner += """help    -> Python's own help system.\n"""
    #o.banner += """object? -> Details about 'object'. ?object also works, ?? prints more.\n"""
    #o.banner += '\nSpock %s -- An interactive Macro Server client.\n' % spockver

    if macro_server is not None and door is not None:
        o.banner += "\nUsing Door %s to access Macro Server %s.\n" % (door,macro_server)
        imacro.InitMacroServer(macro_server, door)            
main()
