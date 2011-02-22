package es.cells.sardana.client.framework.gui.atk.widget.tree;

import es.cells.sardana.client.framework.macroserver.MacroServer;
import fr.esrf.tangoatk.core.Device;

public class MacroServerTreeNode extends SardanaTreeNode {

	public MacroServerTreeNode(MacroServer ms) 
	{
		super(ms);
	}

	public MacroServer getMacroServer()
	{
		return (MacroServer) getUserObject();
	}	
	
	public Device getModel()
	{
		return getMacroServer().getDevice();
	}	
	
	public String getState()
	{
		return getMacroServer().getState();
	}	
}
