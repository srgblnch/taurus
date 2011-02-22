"""
    Macro library containning diffractometer related macros for the macros 
    server Tango device server as part of the Sardana project.
    
"""

from Macro    import *

class _diffrac:
    """Internal class used as a base class for the diffractometer macros"""
    
    two_circle = "two circle"
    four_circle = "four circle"
    
    def prepare(self):
        self.prepared = False
        ctrl_name = self.getEnv('ActiveDiffrac')
        
        if not ctrl_name:
            self.output("No ActiveDiffrac controller defined")
            return
        
        ctrl_list = self.findObjs('.*', type_class=Type.Controller)

        self.ctrl = None
        if not ctrl_name is None:
            ctrl_name = ctrl_name.lower()
            for c in ctrl_list:
                if c.getName().lower() == ctrl_name:
                    self.ctrl = c
        
        if self.ctrl is None:
            self.output("ActiveDiffrac controller not found")
            return
        
        self.pool = self.ctrl.getPool()
        ctrl_klass = self.pool.getCtrlClassByName(self.ctrl.klass)
        self.model = ctrl_klass.model
        
        self.h_mot   = self.ctrl[1]
        self.k_mot   = self.ctrl[2]
        tth_name     = self.h_mot.obj_name_list[0]
        th_name      = self.h_mot.obj_name_list[1]
        self.tth_mot = self.getObj(tth_name, type_class=Type.Motor) 
        self.th_mot  = self.getObj(th_name, type_class=Type.Motor)
        self.h       = self.h_mot.getUserPos()
        self.k       = self.k_mot.getUserPos()
        self.tth     = self.tth_mot.getUserPos()
        self.th      = self.th_mot.getUserPos()
        self.m_list  = [self.tth_mot, self.th_mot]
        self.pm_list = [self.h_mot, self.k_mot]
        self.m_pos   = [self.tth, self.th]
        self.pm_pos  = [self.h, self.k]
        self.omega  = self.th - self.tth/2.0
        
        m = self.model.lower()
        if m == self.two_circle:
            pass
        elif m == self.four_circle:
            self.l_mot    = self.ctrl[3]
            chi_name      = self.h_mot.obj_name_list[2]
            phi_name      = self.h_mot.obj_name_list[3]
            self.chi_mot  = self.getObj(chi_name, type_class=Type.Motor)
            self.phi_mot  = self.getObj(phi_name, type_class=Type.Motor)
            self.l        = self.l_mot.getUserPos()
            self.chi      = self.chi_mot.getUserPos()
            self.phi      = self.phi_mot.getUserPos()
            self.m_list  += [self.chi_mot, self.phi_mot] 
            self.pm_list += [self.l_mot]
            self.m_pos   += [self.chi, self.phi]
            self.pm_pos  += [self.l]
        else:
            self.output("Diffractometer model %s not supported" % self.model)
            return
        
        self.wavelength = self.ctrl.props['wavelength'].value
        self.a = self.ctrl.props['a'].value
        self.b = self.ctrl.props['b'].value
        self.c = self.ctrl.props['c'].value
        self.alpha = self.ctrl.props['alpha'].value
        self.beta = self.ctrl.props['beta'].value
        self.gamma = self.ctrl.props['gamma'].value
        self.prepared = True
        
    def sendSetToCtrl(self,cmd_line):
        self.debug("invoking pool.SendToController('%s', '%s')" % (self.ctrl.name,cmd_line))
        
        try:
            res = self.pool.SendToController([self.ctrl.name,cmd_line])
            if res.lower() != "true":
                 self.output("Send command to controller was NOT successfull!")
                 self.output("Command result was: %s" % res)
                 self.output("Macro execution had no effect")
                 return None
        except:
            self.output("An error occurred when trying to send command to controller")
            raise
        return res

    def set_reflection(self,i,*params):
        reflect_prop_name = self.ctrl.name + "/reflections"
        
        reflections = self.pool.get_property([reflect_prop_name])[reflect_prop_name] or []
        
        str_params = [str(x) for x in params]
        str_params = " ".join(str_params + [self.wavelength])
        cmd_line = 'setor %d %s' % (i,str_params)

        if len(reflections) <= i:
            reflections.append(str_params)
        else:
            reflections[i] = str_params
        
        if not self.sendSetToCtrl(cmd_line) is None:
            self.pool.put_property({reflect_prop_name : reflections})
            
            # update local controller object
            self.ctrl.props['reflections'].value = "\n".join(reflections)

    def set_lat(self,**p):
        props = {}
        for param_name in p.keys():
            props[self.ctrl.name + "/" + param_name] = [str(p[param_name])]

        cmd_line = 'setlat %(a)f %(b)f %(c)f %(alpha)f %(beta)f %(gamma)f' % p

        if not self.sendSetToCtrl(cmd_line) is None:
            self.pool.put_property(props)

            # update local controller object
            for param_name in p.keys():
                self.ctrl.props[param_name].value = str(p[param_name])
        
    def set_direction(self,*params):
        dir_prop_name = self.ctrl.name + "/direction"
        
        str_params = [str(x) for x in params]
        str_params = " ".join(str_params) 
        cmd_line = 'direction %s' % str_params

        if not self.sendSetToCtrl(cmd_line) is None:
            self.pool.put_property({dir_prop_name : [str_params]})
            
            # update local controller object
            self.ctrl.props['direction'].value = str_params

class br(Macro, _diffrac):
    """The br macro is used to move the diffractometer to the reciprocal space 
    coordinates given by H, K and L."""

    param_def = [
       ['H', Type.Float, None, "H value"],
       ['K', Type.Float, None, "K value"],
       ['L', Type.Float, 1.0, "L value"]
    ]
    
    def prepare(self, H, K, L):
        _diffrac.prepare(self)
    
    def run(self, H, K, L):
        if not self.prepared:
            return
        
        param_list = zip(self.pm_list,self.pm_pos)
        self.execMacro('mv',*param_list)


class ubr(Macro, _diffrac):
    """The br macro is used to move the diffractometer to the reciprocal space 
    coordinates given by H, K and L."""

    param_def = [
       ['H', Type.Float, None, "H value"],
       ['K', Type.Float, None, "K value"],
       ['L', Type.Float, 1.0, "L value"]
    ]
    
    def prepare(self, H, K, L):
        _diffrac.prepare(self)
    
    def run(self, H, K, L):
        if not self.prepared:
            return
        
        param_list = zip(self.pm_list,self.pm_pos)
        self.execMacro('umv',*param_list)


class wh(Macro, _diffrac):
    """wh - where, principal axes and reciprocal space
        
    Prints the current reciprocal space coordinates (H K L) and the user 
    positions of the principal motors. Depending on the diffractometer geometry, 
    other parameters such as the angles of incidence and reflection (ALPHA and 
    BETA) and the incident wavelength (LAMBDA) may be displayed."""
    
    def prepare(self):
        _diffrac.prepare(self)
    
    def run(self):
        if not self.prepared:
            return
        
        self.output("")
        pm_pos_str = ["%7.5f" % p for p in self.pm_pos] 
        self.output("%s = %s" % (" ".join(self.ctrl.pm_roles),
                                 " ".join(pm_pos_str)))
        
        m = self.model.lower()
        if m == self.four_circle:
            self.output("Alpha = %s  Beta = %s  Azimuth = %s" % ("[TODO]","[TODO]","[TODO]",))
            
        self.output("Omega = %7.5f  Lambda = %s" %(self.omega,self.wavelength))
        
        self.output("")
        motor_list = self.m_list
        self.execMacro('_wm',*motor_list)

class orn(Macro, _diffrac):
    """Set orientation reflection indicated by the index."""
    
    param_def = [
       ['i', Type.Integer, None, "reflection index (starting at 0)"],
       ['H', Type.Float,   None, "H value"],
       ['K', Type.Float,   None, "K value"],
       ['L', Type.Float,    1.0, "K value"],
    ]
    
    hints = { 'interactive' : 'True' }
    
    def prepare(self, i, H, K, L):
        _diffrac.prepare(self)
    
    def run(self, i, H, K, L):
        if not self.prepared:
            return        
        
        params = self.m_pos + [H, K, L]
        self.set_reflection(i,*params)
        
class or0(Macro, _diffrac):
    """Set primary orientation reflection."""
    
    param_def = [
       ['H', Type.Float, None, "H value"],
       ['K', Type.Float, None, "K value"],
       ['L', Type.Float, 1.0, "L value"],
    ]
    
    hints = { 'interactive' : 'True' }
    
    def prepare(self, H, K, L):
        _diffrac.prepare(self)
    
    def run(self, H, K, L):
        if not self.prepared:
            return        
        
        params = self.m_pos + [H, K, L]
        self.set_reflection(0,*params)

class or1(Macro, _diffrac):
    """Set secondary orientation reflection."""
    
    param_def = [
       ['H', Type.Float, None, "H value"],
       ['K', Type.Float, None, "K value"],
       ['L', Type.Float, 1.0, "L value"],
    ]
    
    hints = { 'interactive' : 'True' }
    
    def prepare(self, H, K, L):
        _diffrac.prepare(self)
    
    def run(self, H, K, L):
        if not self.prepared:
            return
        
        params = self.m_pos + [H, K, L]
        self.set_reflection(1,*params)

class setorn(Macro, _diffrac):
    """Set orientation reflection indicated by the index."""
    
    param_def = [
       ['i', Type.Integer, None, "reflection index (starting at 0)"],
       ['tth', Type.Float, None, "tth value"],
       ['th', Type.Float, None, "th value"],
       ['chi', Type.Float, None, "chi value"],
       ['phi', Type.Float, None, "phi value"],
       ['H', Type.Float, None, "H value"],
       ['K', Type.Float, None, "K value"],
       ['L', Type.Float, None, "L value"]
    ]
    
    hints = { 'interactive' : 'True' }
    
    def prepare(self, i, tth, th, chi, phi, H, K, L):
        _diffrac.prepare(self)
    
    def run(self, i, tth, th, chi, phi, H, K, L):
        if not self.prepared:
            return
        
        params = []
        m = self.model.lower()
        if m == self.two_circle:
            params = [tth, th, H, K, 1.0]
        elif m == self.four_circle:
            params = [tth, th, chi, phi, H, K, L]
            
        self.set_reflection(i,*params)

class setor0(Macro, _diffrac):
    """Set primary orientation reflection."""
    
    param_def = [
       ['tth', Type.Float, None, "tth value"],
       ['th', Type.Float, None, "th value"],
       ['chi', Type.Float, None, "chi value"],
       ['phi', Type.Float, None, "phi value"],
       ['H', Type.Float, None, "H value"],
       ['K', Type.Float, None, "K value"],
       ['L', Type.Float, None, "L value"]
    ]
    
    hints = { 'interactive' : 'True' }
    
    def prepare(self, tth, th, chi, phi, H, K, L):
        _diffrac.prepare(self)
    
    def run(self, tth, th, chi, phi, H, K, L):
        if not self.prepared:
            return
        
        params = []
        m = self.model.lower()
        if m == self.two_circle:
            params = [tth, th, H, K, 1.0]
        elif m == self.four_circle:
            params = [tth, th, chi, phi, H, K, L]
            
        self.set_reflection(0,*params)
        
class setor1(Macro, _diffrac):
    """Set secondary orientation reflection."""
    
    param_def = [
       ['tth', Type.Float, None, "tth value"],
       ['th', Type.Float, None, "th value"],
       ['chi', Type.Float, None, "chi value"],
       ['phi', Type.Float, None, "phi value"],
       ['H', Type.Float, None, "H value"],
       ['K', Type.Float, None, "K value"],
       ['L', Type.Float, None, "L value"]
    ]
    
    hints = { 'interactive' : 'True' }
    
    def prepare(self, tth, th, chi, phi, H, K, L):
        _diffrac.prepare(self)
    
    def run(self, tth, th, chi, phi, H, K, L):
        if not self.prepared:
            return
        
        params = []
        m = self.model.lower()
        if m == self.two_circle:
            params = [tth, th, H, K, 1.0]
        elif m == self.four_circle:
            params = [tth, th, chi, phi, H, K, L]
            
        self.set_reflection(1,*params)        
                    
class setlat(Macro, _diffrac):
    """Set the crystal lattice parameters a, b, c, alpha, beta, gamma.
       for the currently active diffraction pseudo motor controller."""
    
    param_def = [
       ['a', Type.Float, None, "Lattice 'a' parameter"],
       ['b', Type.Float, None, "Lattice 'b' parameter"],
       ['c', Type.Float, None, "Lattice 'c' parameter"],
       ['alpha', Type.Float, None, "Lattice 'alpha' parameter"],
       ['beta',  Type.Float, None, "Lattice 'beta' parameter"],
       ['gamma', Type.Float, None, "Lattice 'gamma' parameter"]
    ]
    
    setup_def = [
       ['setup_param', Type.Float, 23.45, "What should be this bloody value?"]
    ]

    hints = { 'interactive' : 'True' }

    def prepare(self, a, b, c, alpha, beta, gamma):
        _diffrac.prepare(self)
    
    def run(self, a, b, c, alpha, beta, gamma):
        if not self.prepared:
            return
        
        self.set_lat(a=a,b=b,c=c,alpha=alpha,beta=beta,gamma=gamma)

class setaz(Macro, _diffrac):
    """Set the crystal lattice parameters a, b, c, alpha, beta, gamma.
       for the currently active diffraction pseudo motor controller."""
    
    param_def = [
       ['H', Type.Float, None, "H value for the azimutal vector"],
       ['K', Type.Float, None, "K value for the azimutal vector"],
       ['L', Type.Float, None, "L value for the azimutal vector"],
    ]
    
    hints = { 'interactive' : 'True' }
    
    def prepare(self, H, K , L):
        _diffrac.prepare(self)
    
    def run(self, H, K , L):
        if not self.prepared:
            return
        
        self.set_direction(H,K,L)

class H(Macro, _diffrac):
    """x component of the scattering vector."""

    result_def = [
        ['H',  Type.Float, None, 'x component of the scattering vector']
    ]

    def prepare(self):
        _diffrac.prepare(self)
        
    def run(self):
        if not self.prepared:
            return
        
        return self.h

class K(Macro, _diffrac):
    """y component of the scattering vector."""

    result_def = [
        ['K',  Type.Float, None, 'y component of the scattering vector']
    ]

    def prepare(self):
        _diffrac.prepare(self)
        
    def run(self):
        if not self.prepared:
            return

        return self.k

class L(Macro, _diffrac):
    """z component of the scattering vector."""

    result_def = [
        ['L',  Type.Float, None, 'z component of the scattering vector']
    ]

    def prepare(self):
        _diffrac.prepare(self)
        
    def run(self):
        if not self.prepared:
            return

        return self.l
        
class LAMBDA(Macro, _diffrac):
    """x component of the scattering vector."""

    result_def = [
        ['LAMBDA',  Type.Float, None, 'Incident X-ray wavelength lambda.']
    ]

    def prepare(self):
        _diffrac.prepare(self)
        
    def run(self):
        if not self.prepared:
            return
        
        return self.wavelength

class ALPHA(Macro, _diffrac):
    """Incident angle \u03B1 value."""

    result_def = [
        ['ALPHA',  Type.Float, None, 'Incident angle \u03B1.']
    ]

    def prepare(self):
        _diffrac.prepare(self)
        
    def run(self):
        if not self.prepared:
            return

        return self.alpha

class BETA(Macro, _diffrac):
    """Incident angle \u03B2 value."""

    result_def = [
        ['BETA',  Type.Float, None, 'Incident angle \u03B2.']
    ]

    def prepare(self):
        _diffrac.prepare(self)
        
    def run(self):
        if not self.prepared:
            return
        
        return self.beta
    
class GAMMA(Macro, _diffrac):
    """Incident angle \u03B3 value."""

    result_def = [
        ['GAMMA',  Type.Float, None, 'Incident angle \u03B3.']
    ]

    def prepare(self):
        _diffrac.prepare(self)
        
    def run(self):
        if not self.prepared:
            return

        return self.gamma    

class OMEGA(Macro, _diffrac):
    """\u03C9 value. \u03C9 = \u03B8 - (2\u03B8)/2"""

    result_def = [
        ['OMEGA',  Type.Float, None, 'Incident angle \u03C9.']
    ]

    def prepare(self):
        _diffrac.prepare(self)
        
    def run(self):
        if not self.prepared:
            return

        return self.omega
    
class showUB(Macro, _diffrac):
    """Prints the current crystal UB matrix."""

    def prepare(self):
        _diffrac.prepare(self)
        
    def run(self):
        if not self.prepared:
            return
        
        self.output(self.pool.SendToController([self.ctrl.name, '?ub']))
      
class pa(Macro, _diffrac):
    """Prints information about the active diffractometer."""
    
    suffix = ("st","nd","rd","th")
    
    def prepare(self):
        _diffrac.prepare(self)
        
    def run(self):
        if not self.prepared:
            return
        
        mode = self.pool.SendToController([self.ctrl.name, '?operationmode'])
        self.output("%s Geometry, %s" % (self.model, mode))
        self.output("Sector %s" % "[ToDo]")
        self.output("")
        
        reflections = self.ctrl.props['reflections'].value.split('\n')
        m_nb = self.ctrl.nb_mot_roles
        pm_nb = self.ctrl.nb_pm_roles
        for i,reflection in enumerate(reflections):
            parts = reflection.split()
            if i < len(self.suffix): sf = self.suffix[i]
            else: sf = self.suffix[-1]
            self.output("  %d%s Reflection (at lambda %s)" % (i+1,sf,parts[-1]))
            self.output("%24s = %s" % (" ".join(self.ctrl.mot_roles),
                                       " ".join(parts[0:m_nb])))         
            self.output("%24s = %s" % (" ".join(self.ctrl.pm_roles),
                                       " ".join(parts[m_nb:m_nb+pm_nb])))
            self.output("")
        
        self.output("  Lattice Constants (lengths / angles):")
        self.output("%24s = %s %s %s / %s %s %s" % ("real space", self.a, 
                                                    self.b, self.c, self.alpha, 
                                                    self.beta, self.gamma))
        self.output("%24s = %s" % ("reciprocal space","[ToDo]"))
        self.output("")
        
        self.output("  Azimuthal Reference:")
        self.output("%24s = %s" %(" ".join(self.ctrl.pm_roles),"[ToDo]"))
        self.output("")
        self.output("%24s = %s" %("Lambda",self.wavelength))
        self.output("")
        self.output(" Cut Points:")
        self.output("    [ToDo]")
        
class ca(Macro, _diffrac):
    """Calculate motor positions for given H K L according to the current
    operation mode."""
    
    param_def = [
       ['H', Type.Float, None, "H value for the azimutal vector"],
       ['K', Type.Float, None, "K value for the azimutal vector"],
       ['L', Type.Float, None, "L value for the azimutal vector"],
    ]    
    
    def prepare(self, H, K , L):
        _diffrac.prepare(self)    
    
    def run(self, *pars):
        if not self.prepared:
            return
                
        self.output("")
        pm_pos_str = ["%7.5f" % p for p in pars] 
        self.output("%s = %s" % (" ".join(self.ctrl.pm_roles),
                                 " ".join(pm_pos_str)))
        
        m = self.model.lower()
        if m == self.four_circle:
            self.output("Alpha = %s  Beta = %s  Azimuth = %s" % ("[ToDo]","[ToDo]","[ToDo]",))
            
        self.output("Omega = %7.5f  Lambda = %s" %(self.omega,self.wavelength))
        
        self.output("")
        
        cmd_line = "calcA %s" % " ".join([str(p) for p in pars])
        ret = self.pool.SendToController([self.ctrl.name, cmd_line])
        ret = ["%7.5f" % float(p) for p in ret.split()]
        self.output("%24s = %s" % (" ".join(self.ctrl.mot_roles), " ".join(ret)))

class calcA(Macro, _diffrac):
    """Calculate motor positions for current H K L."""
    
    def prepare(self):
        _diffrac.prepare(self)    
    
    def run(self):
        if not self.prepared:
            return
                
        cmd_line = "calcA %s" % " ".join([str(p) for p in self.pm_pos])
        ret = self.pool.SendToController([self.ctrl.name, cmd_line])
        ret = ["%7.5f" % float(p) for p in ret.split()]
        self.output("%24s = %s" % (" ".join(self.ctrl.mot_roles), " ".join(ret)))
        
class calcHKL(Macro, _diffrac):
    """Calculate H K L for current motor positions"""

    def prepare(self):
        _diffrac.prepare(self)
            
    def run(self):
        if not self.prepared:
            return
                
        cmd_line = "calcHKL %s" % " ".join([str(p) for p in self.m_pos])
        ret = self.pool.SendToController([self.ctrl.name,cmd_line])
        ret = ["%9.4f" % float(p) for p in ret.split()]
        self.output("%24s = %s" % (" ".join(self.ctrl.pm_roles), " ".join(ret)))
