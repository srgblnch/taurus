package es.cells.sardana.client.framework.pool;

import java.util.Vector;

public class PseudoCounterClass extends ControllerClass 
{
	Vector<String> counterRoles;
	
	public PseudoCounterClass(String ctrlData)
	{
		super(ctrlData);
	}
	
	public Vector<String> getCounterRoles() 
	{
		return counterRoles;
	}
	
	public void setCounterRoles(Vector<String> counterRoles) 
	{
		this.counterRoles = counterRoles;
	}
	
	public String getCounterRole(int index)
	{
		return counterRoles.get(index);
	}
	
	public boolean match(Object obj) 
	{
		if(!(obj instanceof PseudoCounterClass))
			return false;

		if(!super.match(obj))
			return false;
		
		PseudoCounterClass pcc = (PseudoCounterClass) obj; 
		
		return counterRoles.equals(pcc.counterRoles);
	}	
}
