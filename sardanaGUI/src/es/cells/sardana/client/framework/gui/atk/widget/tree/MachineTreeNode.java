package es.cells.sardana.client.framework.gui.atk.widget.tree;

import es.cells.sardana.client.framework.pool.Machine;


public class MachineTreeNode extends SardanaTreeNode
{

	public MachineTreeNode(Machine userObject)
	{
		super(userObject);
	}

	public Machine getMachine()
	{
		return (Machine)getUserObject();
	}
	
}
