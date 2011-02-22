from Macro    import *

################################################################################
#
# Environment related macros
#
################################################################################

class wenv(Macro):
    """Displays the current value of the given environment variable"""
    
    param_def = [['name', Type.String, None, 'Environment variable name']]

    def run(self, name):
        line = "%s = %s" % (name, self.getEnv(name))
        self.output(line)


class senv(Macro):
    """Sets the given environment variable to the given value"""
    
    param_def = [['name', Type.String, None, 'Environment variable name'],
                 ['value', Type.String, '', 'value']]

    def run(self, name, value):
        self.setEnv(name,value)
        line = "%s = %s" % (name, self.getEnv(name))
        self.output(line)


class waenv(Macro):
    """Displays the current environment"""
    
    def run(self):
        env = self.getAllEnv()
        for env_name in env:
            line = "%s = %s" % (env_name, env[env_name])
            self.output(line)


class lsenv(Macro):
    """Lists all current environment"""
    
    def run(self):
        out = List(['Name','Type','Value','Description'])
        env = self.getAllEnvDescription()
        for env_elem in env:
            obj = env[env_elem]
            out.appendRow([env_elem,obj[0],obj[2],obj[1]])
        for line in out.genOutput():
            self.output(line)