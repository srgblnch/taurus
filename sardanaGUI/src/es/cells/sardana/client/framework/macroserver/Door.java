package es.cells.sardana.client.framework.macroserver;

import es.cells.sardana.client.framework.pool.Machine;
import es.cells.sardana.client.framework.pool.SardanaDevice;


public class Door extends SardanaDevice {

	MacroServer macroServer;
	
	public Door(Machine machine, String name) 
	{
		super(machine, name);
	}

	public void setMacroServer(MacroServer macroServer) 
	{
		this.macroServer = macroServer;
	}

	public MacroServer getMacroServer() {
		return macroServer;
	}
	
}
