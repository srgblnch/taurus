package es.cells.sardana.client.framework.macroserver;

import java.util.List;

import es.cells.sardana.client.framework.pool.DevicePoolUtils;
import es.cells.sardana.client.framework.pool.Machine;
import es.cells.sardana.client.framework.pool.SardanaDevice;


public class MacroServer extends SardanaDevice 
{
	protected List<Door> doors;

	public MacroServer(Machine machine, String name) 
	{
		super(machine, name);
	}
	
	protected void init()
	{
		DevicePoolUtils utils = DevicePoolUtils.getInstance();
		
		setDoors(utils.askForMacroServerDoors(this));
		
		super.init();
	}	
	
	// Door methods -------------------------------------------
	public List<Door> getDoors() 
	{
		return doors;
	}

	public void setDoors(List<Door> doors) 
	{
		this.doors = doors;
	}

	public void clearDoors()
	{
		doors.clear();
	}
	
	public void addDoor(Door door) 
	{
		doors.add(door);	
	}
	
	public boolean hasDoor(Door door) 
	{
		return doors.contains(door);
	}

	public Door getDoor(String door) 
	{
		for (Door doorElem : doors)
			if (doorElem.getName().equals(door))
				return doorElem;
		return null;
	}
	
	public boolean containsDoor(String door)
	{
		return getDoor(door) != null;
	}	

	public Door getDoorByDeviceName(String doorDeviceName) 
	{
		for(Door doorElem : doors)
			if(doorElem.getDeviceName().equalsIgnoreCase(doorDeviceName))
				return doorElem;
		return null;
	}	
}