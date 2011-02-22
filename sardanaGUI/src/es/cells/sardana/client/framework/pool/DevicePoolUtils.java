package es.cells.sardana.client.framework.pool;
 
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.Map;
import java.util.Vector;
import java.util.logging.Level;
import java.util.logging.Logger;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import javax.swing.ImageIcon;

import es.cells.sardana.client.framework.IPreferences;
import es.cells.sardana.client.framework.SardanaManager;
import es.cells.sardana.client.framework.gui.IImageResource;
import es.cells.sardana.client.framework.macroserver.Door;
import es.cells.sardana.client.framework.macroserver.MacroServer;
import es.cells.sardana.client.framework.macroserver.MacroServerCreationInfo;
import es.cells.sardana.client.gui.swing.SwingResource;
import fr.esrf.Tango.AttrDataFormat;
import fr.esrf.Tango.DevFailed;
import fr.esrf.Tango.DevVarLongStringArray;
import fr.esrf.TangoApi.AttributeInfoEx;
import fr.esrf.TangoApi.Database;
import fr.esrf.TangoApi.DbDatum;
import fr.esrf.TangoApi.DbDevImportInfo;
import fr.esrf.TangoApi.DeviceAttribute;
import fr.esrf.TangoApi.DeviceData;
import fr.esrf.TangoApi.DeviceProxy;
import fr.esrf.TangoDs.TangoConst;
import fr.esrf.tangoatk.core.ConnectionException;
import fr.esrf.tangoatk.core.Device;
import fr.esrf.tangoatk.core.DeviceFactory;
import fr.esrf.tangoatk.core.DeviceProperty;

public class DevicePoolUtils {
	
	protected static Logger log = null;
	
	public static final int DFT_REFRESH_INTERVAL = 3000;
	
	public static final String
		MOTOR_ATTR_POSITION = "Position",
		MOTOR_GROUP_ATTR_POSITION = MOTOR_ATTR_POSITION,
		MOTOR_GROUP_ATTR_ELEMENTS = "Elements",
		PSEUDO_MOTOR_ATTR_POSITION = MOTOR_ATTR_POSITION,
		EXP_CHANNEL_VALUE = "Value",
		MEASUREMENT_GROUP_ATTR_INTEGRATION_TIME = "Integration_time",
		MEASUREMENT_GROUP_ATTR_INTEGRATION_COUNT = "Integration_count",
		MEASUREMENT_GROUP_ATTR_TIMER = "Timer",
		MEASUREMENT_GROUP_ATTR_MONITOR = "Monitor",
		MEASUREMENT_GROUP_ATTR_COUNTERS = "Counters",
		MEASUREMENT_GROUP_ATTR_ZERODEXPCHANNELS = "ZeroDExpChannels",
		MEASUREMENT_GROUP_ATTR_PSEUDOCOUNTERS = "PseudoCounters",
		MEASUREMENT_GROUP_ATTR_ONEDEXPCHANNELS = "OneDExpChannels",
		MEASUREMENT_GROUP_ATTR_TWODEXPCHANNELS = "TwoDExpChannels",
		MEASUREMENT_GROUP_ATTR_CHANNELS = "Channels",
		ZEROD_CHANNEL_CUMULATED_VALUE = "CumulatedValue",
		ZEROD_CHANNEL_CUMULATION_TIME = "CumulationTime",
		ZEROD_CHANNEL_CUMULATION_TYPE = "CumulationType",
		POOL_CLASS_NAME = "Pool",
		MACROSERVER_CLASS_NAME = "MacroServer",
		DOOR_CLASS_NAME = "Door",
		POOL_PROP_POOL_PATH = "PoolPath",
		POOL_ATTR_CTRL_LIST = "ControllerList",
		POOL_ATTR_CTRL_CLASS_LIST = "ControllerClassList",
		POOL_ATTR_COM_CHANNEL_LIST = "ComChannelList",
		POOL_ATTR_MOTOR_LIST = "MotorList",
		POOL_ATTR_MOTOR_GROUP_LIST = "MotorGroupList",
		POOL_ATTR_PSEUDO_MOTOR_LIST = "PseudoMotorList",
		POOL_ATTR_PSEUDO_COUNTER_LIST = "PseudoCounterList",
		//POOL_ATTR_PSEUDO_MOTOR_CLASS_LIST = "PseudoMotorClassList",
		POOL_ATTR_EXP_CHANNEL_LIST = "ExpChannelList",
		POOL_ATTR_MEASUREMENT_GROUP_LIST = "MeasurementGroupList",
		POOL_ATTR_SIMULATION_MODE = "SimulationMode",
		POOL_CMD_CREATE_CTRL = "CreateController",
		POOL_CMD_CREATE_COM_CHANNEL = "CreateComChannel",
		POOL_CMD_CREATE_MOTOR = "CreateMotor",
		POOL_CMD_CREATE_MOTOR_GROUP = "CreateMotorGroup",
		//POOL_CMD_CREATE_PSEUDO_MOTOR = "CreatePseudoMotor",
		POOL_CMD_CREATE_EXP_CHANNEL = "CreateExpChannel",
		POOL_CMD_CREATE_MEASUREMENT_GROUP = "CreateMeasurementGroup",
		POOL_CMD_DELETE_CTRL = "DeleteController",
		POOL_CMD_DELETE_COM_CHANNEL = "DeleteComChannel",
		POOL_CMD_DELETE_MOTOR = "DeleteMotor",
		POOL_CMD_DELETE_MOTOR_GROUP = "DeleteMotorGroup",
		POOL_CMD_DELETE_PSEUDO_MOTOR = "DeletePseudoMotor",
		POOL_CMD_DELETE_EXP_CHANNEL = "DeleteExpChannel",
		POOL_CMD_DELETE_MEASUREMENT_GROUP = "DeleteMeasurementGroup",
		POOL_CMD_GET_CTRL_INFO = "GetControllerInfo",
		POOL_CMD_GET_CTRL_INFO_EX = "GetControllerInfoEx",
		//POOL_CMD_GET_PSEUDO_MOTOR_INFO = "GetPseudoMotorInfo",
		POOL_CMD_RELOAD_CONTROLLER_CODE = "ReloadControllerCode",
		//POOL_CMD_RELOAD_PSEUDO_MOTOR_CODE = "ReloadPseudoMotorCode",
		POOL_CMD_INIT_CONTROLLER = "InitController",
		LIST_PATTERN_STR = "Class:\\s*(\\w+)\\s*-\\s*File:\\s*(.+)(.*)",
		CTRL_LIST_PATTERN_STR = "Type:\\s*(\\w+)\\s*-\\s*" + LIST_PATTERN_STR,
		PSEUDO_MOTOR_LIST_PATTERN_STR = LIST_PATTERN_STR,
		ID = "([\\w\\-]+)",
		CTRL_DESCRIPTION_PATTERN_STR = ID+"\\s+\\("+ID+"\\.(\\w+)\\/"+ID+"\\)\\s+-\\s+(\\w+)\\s+(\\w+)\\s+(\\w+)\\s+\\(([\\w\\-\\.]+)\\)(.*)",
		MOTOR_DESCRIPTION_PATTERN_STR = "(\\w+)\\s+\\(((\\w+\\/){2}+(\\w+))\\)(.*)",
		COM_CHANNEL_DESCRIPTION_PATTERN_STR = MOTOR_DESCRIPTION_PATTERN_STR,
	    MOTOR_GROUP_DESCRIPTION_PATTERN_STR = "(\\w+)\\s+\\((([\\w\\.]+\\/){2}+([\\w\\.]+))\\)\\s+Motor list:\\s+([^(]+)(\\((.+)\\))*(.*)",
	    PSEUDO_MOTOR_DESCRIPTION_PATTERN_STR = "(\\w+)\\s+\\((([\\w\\.]+\\/){2}+([\\w\\.]+))\\)\\s+PseudoMotor Motor list:\\s+([^(]+)(\\((.+)\\))*(.*)",
	    PSEUDO_COUNTER_DESCRIPTION_PATTERN_STR = "(\\w+)\\s+\\((([\\w\\.]+\\/){2}+([\\w\\.]+))\\)\\s+PseudoCounter Counter list:\\s*([^(]*)(\\((.+)\\))*(.*)",
	    EXP_CHANNEL_DESCRIPTION_PATTERN_STR ="(\\w+)\\s+\\(((\\w+\\/){2}+(\\w+))\\)\\s+(\\w+)(.*)",
	    MEASUREMENT_GROUP_DESCRIPTION_PATTERN_STR = "(\\w+)\\s+\\((([\\w\\.]+\\/){2}+([\\w\\.]+))\\)\\s+ExpChannel list:\\s+([^(]+)(\\((.+)\\))*(.*)",
	    //PSEUDO_CLASS_PARAM_DESCRIPTION_PATTERN_STR = "((\\w|\\s)+)\\s+:\\s+(.*)",
	    POOL_STATUS_CTRL_ERROR = "Error reported when trying to create controller",
	    POOL_STATUS_CTRL_ERROR_PATTERN_STR = "\\s+(\\w+)\\s+with instance ([\\w\\d]+) from file (\\w+)(.*)"
	    ;
	
	public static final int 
		CTRL_DESCRIPTION_ELEM_COUNT = 8,
		CTRL_DESCRIPTION_NAME_INDEX = 1,
		CTRL_DESCRIPTION_LIB_NAME_INDEX = 2,
		CTRL_DESCRIPTION_LIB_CLASS_NAME_INDEX = 3,
		CTRL_DESCRIPTION_TYPE_INDEX = 5,
		CTRL_DESCRIPTION_LANGUAGE_INDEX = 6,
		CTRL_DESCRIPTION_LIB_INDEX = 8,
		COM_CHANNEL_DESCRIPTION_ELEM_COUNT = 5,
		COM_CHANNEL_DESCRIPTION_NAME_INDEX = 1,
		COM_CHANNEL_DESCRIPTION_TANGO_NAME_INDEX = 2,
		COM_CHANNEL_DESCRIPTION_CTRL_NAME_INDEX = 3,
		COM_CHANNEL_DESCRIPTION_CTRL_NUM_INDEX = 4,
		MOTOR_DESCRIPTION_ELEM_COUNT = 5,
		MOTOR_DESCRIPTION_NAME_INDEX = 1,
		MOTOR_DESCRIPTION_TANGO_NAME_INDEX = 2,
		MOTOR_DESCRIPTION_CTRL_NAME_INDEX = 3,
		MOTOR_DESCRIPTION_CTRL_NUM_INDEX = 4,
		MOTOR_GROUP_DESCRIPTION_ELEM_COUNT = 6,
		MOTOR_GROUP_DESCRIPTION_NAME_INDEX = 1,
		MOTOR_GROUP_DESCRIPTION_TANGO_NAME_INDEX = 2,
		MOTOR_GROUP_DESCRIPTION_SERVER_NAME = 3,
		MOTOR_GROUP_DESCRIPTION_MOTORS_INDEX = 5,
		PSEUDO_MOTOR_DESCRIPTION_ELEM_COUNT = 5,
		PSEUDO_MOTOR_DESCRIPTION_NAME_INDEX = 1,
		PSEUDO_MOTOR_DESCRIPTION_TANGO_NAME_INDEX = 2,
		PSEUDO_MOTOR_DESCRIPTION_LIB_CLASS_NAME_INDEX = 3,
		PSEUDO_MOTOR_DESCRIPTION_MOTORS_INDEX = 5,		
		PSEUDO_COUNTER_DESCRIPTION_ELEM_COUNT = 5,
		PSEUDO_COUNTER_DESCRIPTION_NAME_INDEX = 1,
		PSEUDO_COUNTER_DESCRIPTION_TANGO_NAME_INDEX = 2,
		PSEUDO_COUNTER_DESCRIPTION_LIB_CLASS_NAME_INDEX = 3,
		PSEUDO_COUNTER_DESCRIPTION_COUNTERS_INDEX = 5,		
		POOL_STATUS_CTRL_ERROR_CTRL_CLASS_INDEX = 1,
		POOL_STATUS_CTRL_ERROR_CTRL_INSTANCE_INDEX = 2,
		POOL_STATUS_CTRL_ERROR_CTRL_LIB_INDEX = 3,
		PSEUDO_CLASS_PARAM_NAME = 1,
		PSEUDO_CLASS_PARAM_DESCR = 2,
		PSEUDO_CLASS_PARAM_ELEM_COUNT = 2,
		EXP_CHANNEL_DESCRIPTION_ELEM_COUNT = 6,
		EXP_CHANNEL_DESCRIPTION_NAME_INDEX = 1,
		EXP_CHANNEL_DESCRIPTION_TANGO_NAME_INDEX = 2,
		EXP_CHANNEL_DESCRIPTION_CTRL_NAME_INDEX = 3,
		EXP_CHANNEL_DESCRIPTION_CTRL_NUM_INDEX = 4,
		EXP_CHANNEL_DESCRIPTION_TYPE_INDEX = 5,
		MEASUREMENT_GROUP_DESCRIPTION_ELEM_COUNT = 6,
		MEASUREMENT_GROUP_DESCRIPTION_NAME_INDEX = 1,
		MEASUREMENT_GROUP_DESCRIPTION_TANGO_NAME_INDEX = 2,
		MEASUREMENT_GROUP_DESCRIPTION_SERVER_NAME = 3,
		MEASUREMENT_GROUP_DESCRIPTION_CHANNELS_INDEX = 5
		;
	
	
	public static final int[]
	    CTRL_DESCRIPTION_ELEMS = new int[]{
			CTRL_DESCRIPTION_NAME_INDEX,
			CTRL_DESCRIPTION_LIB_NAME_INDEX,
			CTRL_DESCRIPTION_LIB_CLASS_NAME_INDEX,
			CTRL_DESCRIPTION_TYPE_INDEX,
			CTRL_DESCRIPTION_LANGUAGE_INDEX,
			CTRL_DESCRIPTION_LIB_INDEX
		},
	    COM_CHANNEL_DESCRIPTION_ELEMS = new int[]{
			COM_CHANNEL_DESCRIPTION_NAME_INDEX,
			COM_CHANNEL_DESCRIPTION_TANGO_NAME_INDEX,
			COM_CHANNEL_DESCRIPTION_CTRL_NUM_INDEX
		},
		MOTOR_DESCRIPTION_ELEMS = new int[]{
			MOTOR_DESCRIPTION_NAME_INDEX,
			MOTOR_DESCRIPTION_TANGO_NAME_INDEX,
			MOTOR_DESCRIPTION_CTRL_NUM_INDEX
		},
		MOTOR_GROUP_DESCRIPTION_ELEMS = new int[]{
			MOTOR_GROUP_DESCRIPTION_NAME_INDEX,
			MOTOR_GROUP_DESCRIPTION_TANGO_NAME_INDEX,
			MOTOR_GROUP_DESCRIPTION_MOTORS_INDEX
		},
		PSEUDO_MOTOR_DESCRIPTION_ELEMS = new int[]{
			PSEUDO_MOTOR_DESCRIPTION_NAME_INDEX,
			PSEUDO_MOTOR_DESCRIPTION_TANGO_NAME_INDEX,
			PSEUDO_MOTOR_DESCRIPTION_MOTORS_INDEX
		},
		PSEUDO_CLASS_PARAM_DESCRIPTION_ELEMS = new int[]{
			PSEUDO_CLASS_PARAM_NAME,
			PSEUDO_CLASS_PARAM_DESCR
		};
	
	public static final Pattern
		LIST_PATTERN = Pattern.compile(LIST_PATTERN_STR),	
		CTRL_LIST_PATTERN = Pattern.compile(CTRL_LIST_PATTERN_STR),
		CTRL_DESCRIPTION_PATTERN = Pattern.compile(CTRL_DESCRIPTION_PATTERN_STR),
		MOTOR_DESCRIPTION_PATTERN = Pattern.compile(MOTOR_DESCRIPTION_PATTERN_STR),
		COM_CHANNEL_DESCRIPTION_PATTERN = Pattern.compile(COM_CHANNEL_DESCRIPTION_PATTERN_STR),
		MOTOR_GROUP_DESCRIPTION_PATTERN = Pattern.compile(MOTOR_GROUP_DESCRIPTION_PATTERN_STR),
		PSEUDO_MOTOR_DESCRIPTION_PATTERN = Pattern.compile(PSEUDO_MOTOR_DESCRIPTION_PATTERN_STR),
		PSEUDO_COUNTER_DESCRIPTION_PATTERN = Pattern.compile(PSEUDO_COUNTER_DESCRIPTION_PATTERN_STR),
		PSEUDO_MOTOR_LIST_PATTERN = Pattern.compile(PSEUDO_MOTOR_LIST_PATTERN_STR),
		POOL_STATUS_CTRL_ERROR_PATTERN = Pattern.compile(POOL_STATUS_CTRL_ERROR_PATTERN_STR, Pattern.DOTALL),
		POOL_STATUS_CTRL_ERROR_SPLIT_PATTERN = Pattern.compile(POOL_STATUS_CTRL_ERROR, Pattern.DOTALL),
		EXP_CHANNEL_DESCRIPTION_PATTERN = Pattern.compile(EXP_CHANNEL_DESCRIPTION_PATTERN_STR),
		MEASUREMENT_GROUP_DESCRIPTION_PATTERN = Pattern.compile(MEASUREMENT_GROUP_DESCRIPTION_PATTERN_STR),
		//PSEUDO_CLASS_PARAM_DESCRIPTION_PATTERN = Pattern.compile(PSEUDO_CLASS_PARAM_DESCRIPTION_PATTERN_STR),
		POOL_PATH_PATTERN = Pattern.compile(":");

	public static String[] logging_level = { "OFF" , "FATAL" , "ERROR" , "WARNING" , "INFO" , "DEBUG" };
	
	protected static DevicePoolUtils instance = null;
	
	protected DevicePoolUtils()
	{
	}
	
	static public DevicePoolUtils getInstance()
	{
		if(instance == null)
		{
			instance = new DevicePoolUtils();
			instance.init();
		}
		return instance;
	}
	
	protected void init()
	{
		log = SardanaManager.getInstance().getLogger(this.getClass().getName());
		log.setLevel(Level.ALL);
		
		if(System.getProperty("TANGO_HOST") == null)
		{
			IPreferences pref = SardanaManager.getInstance().getLoadedPreferences();
			String hostName = pref.getTangoHostName();
			String hostPort = pref.getTangoHostPort();
				
			System.setProperty("TANGO_HOST", hostName + ":" + hostPort);
			
			log.fine("Setting system property TANGO_HOST to " + hostName + ":" + hostPort);
		}
	}
	
	public String getDevicePoolClassName()
	{
		return POOL_CLASS_NAME;
	}

	public String getMacroServerClassName()
	{
		return MACROSERVER_CLASS_NAME;
	}

	public String getDoorClassName()
	{
		return DOOR_CLASS_NAME;
	}
	
	public String[] getMacroServerInstances(Machine machine)
	{
		if(machine == null) return new String[0];
		
		Database db = machine.getDataBase();
		
		if(db == null)
			return new String[0];
		
		try 
		{
			return db.get_instance_name_list( getMacroServerClassName() );
		} 
		catch (DevFailed e) 
		{
			log.severe("Failed to get Macro servers from database: " + e.getMessage());
			return new String[0];
		}
	}
	
	public String[] getDevicePoolServerInstances(Machine machine)
	{
		if(machine == null) return new String[0];
		
		Database db = machine.getDataBase();
		
		if(db == null)
			return new String[0];
		
		try 
		{
			return db.get_instance_name_list( getDevicePoolClassName() );
		} 
		catch (DevFailed e) 
		{
			log.severe("Failed to get Device Pool servers from database: " + e.getMessage());
			return new String[0];
		}
	}
	
	public String[] getServerInstanceClasses(Machine machine, String fullServerInstanceName)
	{
		Database db = machine.getDataBase();
		
		if(db == null)
			return new String[0];
		
		try 
		{
			return db.get_server_class_list(fullServerInstanceName);
		} 
		catch (DevFailed e) 
		{
			log.severe("Failed to get Device Server " + fullServerInstanceName + " Classes: " + e.getMessage());
			return new String[0];
		}		
	}
	
	public String[] getDevices(Machine machine, String fullServerInstanceName, String className)
	{
		Database db = machine.getDataBase();
		
		if(db == null)
			return new String[0];
		
		try 
		{
			return db.get_device_name(fullServerInstanceName, className);
		} 
		catch (DevFailed e) 
		{
			log.severe("Failed to get Device " + fullServerInstanceName + " from database: " + e.getMessage());
			return new String[0];
		}			
	}
	
	public String[] getMacroServerDevices(Machine machine, String fullServerInstanceName)
	{
		return getDevices(machine, fullServerInstanceName, getMacroServerClassName());
	}
	
	public ArrayList<String> getMacroServerDevices(Machine machine)
	{
		String[] macroServers = getMacroServerInstances(machine);
		
		ArrayList<String> macroDevices = new ArrayList<String>();
		
		for(String macroServer : macroServers)
		{
			String[] serverMacroDevices = getMacroServerDevices(machine, getMacroServerClassName() + "/" + macroServer);
			Collections.addAll(macroDevices, serverMacroDevices);
		}
		return macroDevices;
	}
	
	public ArrayList<String> getAvailableMacroServers(Machine machine)
	{
		Database db = machine.getDataBase();
		
		ArrayList<String> ret = new ArrayList<String>();
		
		if(db == null)
			return ret;
		
		try 
		{
			Collections.addAll( ret,	
					db.get_device_exported_for_class( getMacroServerClassName() ) );
			return ret;
		} 
		catch (DevFailed e) 
		{
			log.severe("Failed to get available Macro servers from database: " + e.getMessage());
			return ret;
		}
	}
	
	public List<Door> askForMacroServerDoors(MacroServer macroServer)
	{
		log.entering("DPUtils","askForMacroServerDoors",macroServer);
		ArrayList<Door> ret = new ArrayList<Door>();
		
		String msName = macroServer.getDeviceName();
		try 
		{
			Database db = macroServer.getDataBase();

			String[] doorNames = db.get_device_name("%", getDoorClassName());
			
			for(String doorName : doorNames)
			{
				DbDatum data = db.get_device_property(doorName,"MacroServerName");
				String macroServerName = data.extractString();
				
				if(macroServerName.equalsIgnoreCase(msName))
				{
					Door door = getNewDoor(macroServer, doorName);
					ret.add(door);
				}
			}
			log.exiting("DPUtils","askForMacroServerDoors",ret);
			return ret;
		} 
		catch (DevFailed e) 
		{
			log.severe("Failed to ask MacroServer " + macroServer + " for its doors: " + e.getMessage());
			return ret;
		}
	}

	private Door getNewDoor(MacroServer macroServer, String doorName) 
	{
		Door ret = new Door(macroServer.getMachine(),doorName);
		ret.setMacroServer(macroServer);
		return ret;
	}

	public String[] getPoolDevices(Machine machine, String fullServerInstanceName)
	{
		return getDevices(machine, fullServerInstanceName, getDevicePoolClassName());
	}
	
	public ArrayList<String> getPoolDevices(Machine machine)
	{
		String[] poolServers = getDevicePoolServerInstances(machine);
		
		ArrayList<String> poolDevices = new ArrayList<String>();
		
		for(String poolServer : poolServers)
		{
			String[] serverPoolDevices = getPoolDevices(machine, getDevicePoolClassName() + "/" + poolServer);
			Collections.addAll(poolDevices, serverPoolDevices);
		}
		
		return poolDevices;
	}
	
	public ArrayList<String> getAvailableDevicePools(Machine machine)
	{
		Database db = machine.getDataBase();
		
		ArrayList<String> ret = new ArrayList<String>();
		
		if(db == null)
			return ret;
		
		try 
		{
			Collections.addAll( ret,	
					db.get_device_exported_for_class( getDevicePoolClassName() ) );
			return ret;
		} 
		catch (DevFailed e) 
		{
			log.severe("Failed to get available Device Pools from database: " + e.getMessage());
			return ret;
		}
	}
	
	protected DeviceProperty askForDevicePoolPath(DevicePool pool) 
	{
		DeviceProperty ret = null;

		Device device = pool.getDevice();
		
		if(device == null)
			device = askForDevice(pool.getMachine(), pool.getName());
		
		if(device == null)
			return ret;
		
		device.refreshPropertyMap();
		ret = device.getProperty(POOL_PROP_POOL_PATH);
		
		return ret;
	}
	
	protected List<DeviceProperty> askForDevicePoolProperties(DevicePool pool)
	{
		log.entering("DPUtils","askForDevicePoolProperties",pool);
		List<DeviceProperty> ret = new ArrayList<DeviceProperty>();
		
		Device device = pool.getDevice();
		
		if(device == null)
			device = askForDevice(pool.getMachine(), pool.getName());
		
		if(device == null)
		{
			log.exiting("DPUtils","askForDevicePoolProperties (no device)");
			return ret;
		}
		
		device.refreshPropertyMap();
		Map props = device.getPropertyMap();
		
		for(Object obj : props.values())
		{
			DeviceProperty prop = (DeviceProperty) obj;
			if(prop.getName().equalsIgnoreCase("Controllers"))
				continue;
			ret.add(prop);
		}
		log.exiting("DPUtils","askForDevicePoolProperties",ret);
		return ret;
	}
	
	protected ArrayList<Controller> askForDevicePoolControllers(DevicePool pool)
	{
		log.entering("DPUtils","askForDevicePoolControllers",pool);
		ArrayList<Controller> ret = new ArrayList<Controller>();
		
		try 
		{
			Device device = pool.getDevice();
			
			if(device == null)
				device = askForDevice(pool.getMachine(), pool.getName());
			
			if(device == null)
			{
				log.exiting("DPUtils","askForDevicePoolControllers (No device)");
				return ret;
			}
			if(device.isAlive() == false)
			{
				log.exiting("DPUtils","askForDevicePoolControllers (Not alive)");
				return ret;
			}
			
			DeviceAttribute attribute = device.read_attribute(POOL_ATTR_CTRL_LIST);
			
			String[] ctrlData = attribute.extractStringArray();
			
			for(String ctrlElem : ctrlData)
			{
				Controller ctrl = getNewController(pool, ctrlElem);
				ret.add(ctrl);
			}
			log.exiting("DPUtils","askForDevicePoolControllers",ret);
			return ret;
		} 
		catch (DevFailed e) 
		{
			log.severe("Failed to ask Device Pool " + pool + " for its controllersNode: " + e.getMessage());
			return ret;
		}
	}

	protected ArrayList<CommunicationChannel> 
	askForDevicePoolCommunicationChannels(DevicePool pool)
	{
		log.entering("DPUtils","askForDevicePoolCommunicationChannels",pool);
		ArrayList<CommunicationChannel> ret = new ArrayList<CommunicationChannel>();
		
		try 
		{
			Device device = pool.getDevice();
			
			if(device == null)
				device = askForDevice(pool.getMachine(), pool.getName());
			
			if(device == null)
			{
				log.exiting("DPUtils","askForDevicePoolCommunicationChannels (No device)");
				return ret;
			}
			if(device.isAlive() == false)
			{
				log.exiting("DPUtils","askForDevicePoolCommunicationChannels (Not alive)");
				return ret;
			}
			
			DeviceAttribute attribute = device.read_attribute(POOL_ATTR_COM_CHANNEL_LIST);
			
			String[] comChannelData = attribute.extractStringArray();
			
			for(String comChannelElem : comChannelData)
			{
				CommunicationChannel comChannel = getNewCommunicationChannel(pool, comChannelElem);
				ret.add(comChannel);
				
				// Add motorGroup information to controller
				Controller ctrl = comChannel.getController();
				
				if(ctrl != null)
				{
					ctrl.addElement(comChannel);
				}
			}
			log.exiting("DPUtils","askForDevicePoolCommunicationChannels",ret);
			return ret;
		} 
		catch (DevFailed e) 
		{
			log.severe("Failed to ask Device Pool " + pool + " for its motorsNode: " + e.getMessage());
			return ret;
		}
	}
	
	protected ArrayList<Motor> askForDevicePoolMotors(DevicePool pool)
	{
		log.entering("DPUtils","askForDevicePoolMotors",pool);
		ArrayList<Motor> ret = new ArrayList<Motor>();
		
		try 
		{
			Device device = pool.getDevice();
			
			if(device == null)
				device = askForDevice(pool.getMachine(), pool.getName());
			
			if(device == null)
			{
				log.exiting("DPUtils","askForDevicePoolMotors (No device)");
				return ret;
			}
			if(device.isAlive() == false)
			{
				log.exiting("DPUtils","askForDevicePoolMotors (Not alive)");
				return ret;
			}
			
			DeviceAttribute attribute = device.read_attribute(POOL_ATTR_MOTOR_LIST);
			
			String[] motorData = attribute.extractStringArray();
			
			for(String motorElem : motorData)
			{
				Motor motor = getNewMotor(pool, motorElem);
				ret.add(motor);
				
				// Add motorGroup information to controller
				Controller ctrl = motor.getController();
				
				if(ctrl != null)
				{
					ctrl.addElement(motor);
				}
			}
			log.exiting("DPUtils","askForDevicePoolMotors",ret);
			return ret;
		} 
		catch (DevFailed e) 
		{
			log.severe("Failed to ask Device Pool " + pool + " for its motorsNode: " + e.getMessage());
			return ret;
		}
	}
	
	protected List<PseudoMotor> askForDevicePoolPseudoMotors(DevicePool pool)
	{
		log.entering("DPUtils","askForDevicePoolPseudoMotors",pool);
		ArrayList<PseudoMotor> ret = new ArrayList<PseudoMotor>();
		
		try 
		{
			Device device = pool.getDevice();
			
			if(device == null)
				device = askForDevice(pool.getMachine(), pool.getName());
			
			if(device == null)
			{
				log.exiting("DPUtils","askForDevicePoolPseudoMotors (No device)");
				return ret;
			}
			if(device.isAlive() == false)
			{
				log.exiting("DPUtils","askForDevicePoolPseudoMotors (Not alive)");
				return ret;
			}
			
			DeviceAttribute attribute = device.read_attribute(POOL_ATTR_PSEUDO_MOTOR_LIST);
			
			String[] motorData = attribute.extractStringArray();
			
			for(String motorElem : motorData)
			{
				PseudoMotor pseudoMotor = getNewPseudoMotor(pool, motorElem);
				ret.add(pseudoMotor);
			}
			log.exiting("DPUtils","askForDevicePoolPseudoMotors",ret);
			return ret;
		} 
		catch (DevFailed e) 
		{
			log.severe("Failed to ask Device Pool " + pool + " for its pseudo motors Node: " + e.getMessage());
			return ret;
		}
	}
	
	protected List<PseudoCounter> askForDevicePoolPseudoCounters(DevicePool pool)
	{
		log.entering("DPUtils","askForDevicePoolPseudoCounters",pool);
		ArrayList<PseudoCounter> ret = new ArrayList<PseudoCounter>();
		
		try 
		{
			Device device = pool.getDevice();
			
			if(device == null)
				device = askForDevice(pool.getMachine(), pool.getName());
			
			if(device == null)
			{
				log.exiting("DPUtils","askForDevicePoolPseudoCounters (No device)");
				return ret;
			}
			if(device.isAlive() == false)
			{
				log.exiting("DPUtils","askForDevicePoolPseudoCounters (Not alive)");
				return ret;
			}
			
			DeviceAttribute attribute = device.read_attribute(POOL_ATTR_PSEUDO_COUNTER_LIST);
			
			String[] counterData = attribute.extractStringArray();
			
			for(String counterElem : counterData)
			{
				PseudoCounter pseudoCounter = getNewPseudoCounter(pool, counterElem);
				ret.add(pseudoCounter);
			}
			log.exiting("DPUtils","askForDevicePoolPseudoCounters",ret);
			return ret;
		} 
		catch (DevFailed e) 
		{
			log.severe("Failed to ask Device Pool " + pool + " for its pseudo counters Node: " + e.getMessage());
			return ret;
		}
	}	
	
	public List<ControllerClass> askForDevicePoolControllerClasses(DevicePool pool)
	{
		log.entering("DPUtils","askForDevicePoolControllerClasses",pool);
		ArrayList<ControllerClass> ret = new ArrayList<ControllerClass>();
		
		try 
		{
			Device device = pool.getDevice();
			
			if(device == null)
				device = askForDevice(pool.getMachine(), pool.getName());
			
			if(device == null)
			{
				log.exiting("DPUtils","askForDevicePoolControllerClasses (No device)");
				return ret;
			}
			if(device.isAlive() == false)
			{
				log.exiting("DPUtils","askForDevicePoolControllerClasses (Not alive)");
				return ret;
			}
			 
			DeviceAttribute attribute = device.read_attribute(POOL_ATTR_CTRL_CLASS_LIST);
			
			String[] ctrlClassArray = attribute.extractStringArray();
			
			for(String ctrlClassStr : ctrlClassArray)
			{
				ControllerClass ctrlClass = null;
				if(ctrlClassStr.startsWith("Type: PseudoMotor"))
					ctrlClass = new PseudoMotorClass(ctrlClassStr);
				else if(ctrlClassStr.startsWith("Type: PseudoCounter"))
					ctrlClass = new PseudoCounterClass(ctrlClassStr);
				else
					ctrlClass = new ControllerClass(ctrlClassStr);
				
				ctrlClass.setPool(pool);
				
				if(ctrlClass.getType() == ControllerType.InvalidControllerType)
				{
					log.warning("Unable to determine type for controller class: " + ctrlClass.getClassName() + " from " + ctrlClassStr);
				}
				if(ctrlClass.getLanguage() == ControllerLanguage.InvalidCtrlLanguage)
				{
					log.warning("Unable to determine language for controller class: " + ctrlClass.getClassName() + " from " + ctrlClassStr);
				}
				
				DeviceData args = new DeviceData();
				args.insert(new String[] {
						ctrlClass.getType().toString(),
						ctrlClass.getFileName(), 
						ctrlClass.getClassName()});
				
				try
				{
					DeviceData res = device.executeCommand(DevicePoolUtils.POOL_CMD_GET_CTRL_INFO_EX, args);
					
					byte[] resArray = res.extractByteArray();
					
					int pos = 0;
					
					String description = getStringFromByteArray(resArray, pos, '\0');
					pos += description.length() + 1;
					ctrlClass.setDescription(description);
					
					String gender = getStringFromByteArray(resArray, pos, '\0');
					pos += gender.length() + 1;
					ctrlClass.setGender(gender);

					String model = getStringFromByteArray(resArray, pos, '\0');
					pos += model.length() + 1;
					ctrlClass.setModel(model);

					String organization = getStringFromByteArray(resArray, pos, '\0');
					pos += organization.length() + 1;
					ctrlClass.setOrganization(organization);					
					
					String propNbStr = getStringFromByteArray(resArray, pos, '\0');
					pos += propNbStr.length() + 1;
					
					int propNb = 0;
						
					try
					{
						propNb = Integer.valueOf(propNbStr);
					}
					catch(NumberFormatException nfe)
					{
						log.warning("Failed to get property numbers for " + ctrlClassStr);
					}

					DbDatum[] classProperties = new DbDatum[propNb];
					for(int i = 0 ; i < propNb; i++)
					{
						PropertyInfo prop = new PropertyInfo();
						
						String name = getStringFromByteArray(resArray, pos, '\0');
						pos += name.length() + 1;
						prop.setName(name);
						String type = getStringFromByteArray(resArray, pos, '\0');
						pos += type.length() + 1;
						prop.setType(type);
						String descr = getStringFromByteArray(resArray, pos, '\0');
						pos += descr.length() + 1;
						prop.setDescription(descr);
						String dftVal = getStringFromByteArray(resArray, pos, '\0');
						pos += dftVal.length() + 1;
						if(dftVal.length() > 0)
							prop.setDefaultValue(dftVal);
						
						ctrlClass.addPropertyInfo(prop);
						classProperties[i] = new DbDatum(prop.getName());
					}
					classProperties = pool.getDataBase().get_property(ctrlClass.getClassName(), classProperties);
					ctrlClass.addDbClassProperties(classProperties);
				

					if(ctrlClass.getType() == ControllerType.PseudoMotor)
					{	
						PseudoMotorClass pmClass = (PseudoMotorClass) ctrlClass;
						String motor_role_count_str = getStringFromByteArray(resArray, pos, '\0');
						pos += motor_role_count_str.length() + 1;
						int motor_role_count = Integer.parseInt(motor_role_count_str);
						Vector<String> motor_roles = new Vector<String>(motor_role_count);
						
						for(int i = 0; i < motor_role_count; i++)
						{
							String motor_role = getStringFromByteArray(resArray, pos, '\0');
							pos += motor_role.length() + 1;
							motor_roles.add(motor_role);
						}

						String pseudo_motor_role_count_str = getStringFromByteArray(resArray, pos, '\0');
						pos += pseudo_motor_role_count_str.length() + 1;
						int pseudo_motor_role_count = Integer.parseInt(pseudo_motor_role_count_str);
						Vector<String> pseudo_motor_roles = new Vector<String>(pseudo_motor_role_count);
						
						for(int i = 0; i < pseudo_motor_role_count; i++)
						{
							String pseudo_motor_role = getStringFromByteArray(resArray, pos, '\0');
							pos += pseudo_motor_role.length() + 1;
							pseudo_motor_roles.add(pseudo_motor_role);
						}
						
						pmClass.setPseudoMotorRoles(pseudo_motor_roles);
						pmClass.setMotorRoles(motor_roles);
					}
					else if(ctrlClass.getType() == ControllerType.PseudoCounter)
					{
						PseudoCounterClass pcClass = (PseudoCounterClass) ctrlClass;
						String counter_role_count_str = getStringFromByteArray(resArray, pos, '\0');
						pos += counter_role_count_str.length() + 1;
						int counter_role_count = Integer.parseInt(counter_role_count_str);
						Vector<String> counter_roles = new Vector<String>(counter_role_count);
						
						for(int i = 0; i < counter_role_count; i++)
						{
							String counter_role = getStringFromByteArray(resArray, pos, '\0');
							pos += counter_role.length() + 1;
							counter_roles.add(counter_role);
						}
						
						pcClass.setCounterRoles(counter_roles);
					}
					
					String imgFilename = getStringFromByteArray(resArray, pos, '\0');
					pos += imgFilename.length() + 1;
					ctrlClass.setImageFileName(imgFilename);
					
					String imgFileSizeStr = getStringFromByteArray(resArray, pos, '\0');
					pos += imgFileSizeStr.length() + 1;
					int imgFileSize = Integer.parseInt(imgFileSizeStr);
					ctrlClass.setImageFileSize(imgFileSize);

					String logoFilename = getStringFromByteArray(resArray, pos, '\0');
					pos += logoFilename.length() + 1;
					ctrlClass.setLogoFileName(logoFilename);
					
					String logoFileSizeStr = getStringFromByteArray(resArray, pos, '\0');
					pos += logoFileSizeStr.length() + 1;
					int logoFileSize = Integer.parseInt(logoFileSizeStr);
					ctrlClass.setLogoFileSize(logoFileSize);

					String iconFilename = getStringFromByteArray(resArray, pos, '\0');
					pos += iconFilename.length() + 1;
					ctrlClass.setIconFileName(iconFilename);
					
					String iconFileSizeStr = getStringFromByteArray(resArray, pos, '\0');
					pos += iconFileSizeStr.length() + 1;
					int iconFileSize = Integer.parseInt(iconFileSizeStr);
					ctrlClass.setIconFileSize(iconFileSize);  

					if(!imgFilename.endsWith("(invalid)") && 
					   imgFilename.length() > 0 &&
					   imgFileSize > 0)
					{
						byte[] imgData = new byte[imgFileSize];
						System.arraycopy(resArray, pos, imgData, 0, imgFileSize);
						ctrlClass.setImage(new ImageIcon(imgData));
					}
					pos += imgFileSize;
					
					if(!logoFilename.endsWith("(invalid)") &&
					   logoFilename.length() > 0 &&
					   logoFileSize > 0)
					{
						byte[] logoData = new byte[logoFileSize];
						System.arraycopy(resArray, pos, logoData, 0, logoFileSize);
						ctrlClass.setLogo(new ImageIcon(logoData));
					}
					pos += logoFileSize;
					
					if(!iconFilename.endsWith("(invalid)") &&
							iconFilename.length() > 0 &&
							iconFileSize > 0)
					{
						byte[] iconData = new byte[iconFileSize];
						System.arraycopy(resArray, pos, iconData, 0, iconFileSize);
						ImageIcon ii = new ImageIcon(iconData);
						ctrlClass.setIcon(ii);
						SwingResource.addResource(ctrlClass.getClassName(), ii);
					}
					else
					{
						String iconName = IImageResource.getControllerTypeIcon(ctrlClass.getType());
						SwingResource.addResource(ctrlClass.getClassName(), 
								SwingResource.bigIconMap.get(iconName));
					}
						
				}
				catch(DevFailed e1) 
				{
					log.warning("Failed to ask Device Pool " + pool + " for info-ex on controller class '" + ctrlClass.getClassName() + "': " + e1.getMessage());
				
					try
					{
						DeviceData res = device.executeCommand(DevicePoolUtils.POOL_CMD_GET_CTRL_INFO, args);
						
						String[] resArray = res.extractStringArray();
						
						ctrlClass.setDescription(resArray[0]);
						
						ctrlClass.setGender(resArray[1]);
						ctrlClass.setModel(resArray[2]);
						ctrlClass.setOrganization(resArray[3]);
						
						int propNb = Integer.parseInt(resArray[4]);
						DbDatum[] classProperties = new DbDatum[propNb];
						for(int i = 5, j = 0 ; i <  (propNb*4) + 2; i++)
						{
							PropertyInfo prop = new PropertyInfo();
							
							prop.setName(resArray[i++]);
							prop.setType(resArray[i++]);
							prop.setDescription(resArray[i++]);
							if(resArray[i].length() > 0)
								prop.setDefaultValue(resArray[i]);
							
							ctrlClass.addPropertyInfo(prop);
							classProperties[j++] = new DbDatum(prop.getName());
						}
						
						classProperties = pool.getDataBase().get_property(ctrlClass.getClassName(), classProperties);
						
						ctrlClass.addDbClassProperties(classProperties);
					}
					catch (DevFailed e) 
					{
						log.severe("Failed to ask Device Pool " + pool + " for info on controller class '" + ctrlClass.getClassName() + "': " + e.getMessage());
					}
				}
				
				ret.add( ctrlClass );
			}
			log.exiting("DPUtils","askForDevicePoolControllerClasses",ret);
			return ret;
		}
		catch (DevFailed e) 
		{
			String desc = devFailedToString(e);
			log.severe("Failed to ask Device Pool " + pool + " for its controller classes: " + desc);
			return ret;
		}
	}

	protected String getStringFromByteArray(byte array[], int start, char terminator)
	{
		StringBuffer buff = new StringBuffer();
		int i = start;
		while(i < array.length && array[i] != terminator)
			buff.append((char)array[i++]);
		
		return buff.toString();
	}

	protected ArrayList<MotorGroup> askForDevicePoolMotorGroups(DevicePool pool)
	{
		log.entering("DPUtils","askForDevicePoolMotorGroups",pool);
		ArrayList<MotorGroup> ret = new ArrayList<MotorGroup>();
		
		try 
		{
			Device device = pool.getDevice();
			
			if(device == null)
				device = askForDevice(pool.getMachine(), pool.getName());
			
			if(device == null)
			{
				log.exiting("DPUtils","askForDevicePoolMotorGroups (No device)");
				return ret;
			}
			if(device.isAlive() == false)
			{
				log.exiting("DPUtils","askForDevicePoolMotorGroups (Not alive)");
				return ret;
			}
			
			DeviceAttribute attribute = device.read_attribute(POOL_ATTR_MOTOR_GROUP_LIST);
			
			String[] motorGroupData = attribute.extractStringArray();
			
			for(String motorGroupElem : motorGroupData)
			{
				MotorGroup motorGroup = getNewEmptyMotorGroup(pool, motorGroupElem);
				ret.add(motorGroup);
			}
			
			int index = 0;
			for(MotorGroup motorGroup : ret)
			{
				fillEmptyMotorGroup(motorGroup, motorGroupData[index++], ret);
			}
			log.exiting("DPUtils","askForDevicePoolMotorGroups",ret);
			return ret;

		} 
		catch (DevFailed e) 
		{
			log.severe("Failed to ask Device Pool " + pool + " for its motorGroup groups: " + e.getMessage());
			return ret;
		}
	}
	
	protected ArrayList<ExperimentChannel> askForDevicePoolExperimentChannels(DevicePool pool)
	{
		log.entering("DPUtils","askForDevicePoolExperimentChannels",pool);
		ArrayList<ExperimentChannel> ret = new ArrayList<ExperimentChannel>();
		
		try 
		{
			Device device = pool.getDevice();
			
			if(device == null)
				device = askForDevice(pool.getMachine(), pool.getName());
			
			if(device == null)
			{
				log.exiting("DPUtils","askForDevicePoolExperimentChannels (No device)");
				return ret;
			}
			if(device.isAlive() == false)
			{
				log.exiting("DPUtils","askForDevicePoolExperimentChannels (Not alive)");
				return ret;
			}
			
			DeviceAttribute attribute = device.read_attribute(POOL_ATTR_EXP_CHANNEL_LIST);
			
			String[] channelData = attribute.extractStringArray();
			
			for(String channelElem : channelData)
			{
				ExperimentChannel channel = getNewExperimentChannel(pool, channelElem);
				ret.add(channel);
				
				// Add channel information to controller
				Controller ctrl = channel.getController();
				
				if(ctrl != null)
				{
					ctrl.addElement(channel);
				}
			}
			log.exiting("DPUtils","askForDevicePoolExperimentChannels",ret);
			return ret;
		} 
		catch (DevFailed e) 
		{
			log.severe("Failed to ask Device Pool " + pool + " for its experiment channels: " + e.getMessage());
			return ret;
		}
	}
	
	protected ArrayList<MeasurementGroup> askForDevicePoolMeasurementGroups(DevicePool pool)
	{
		log.entering("DPUtils","askForDevicePoolMeasurementGroups",pool);
		ArrayList<MeasurementGroup> ret = new ArrayList<MeasurementGroup>();
		
		try 
		{
			Device device = pool.getDevice();
			
			if(device == null)
				device = askForDevice(pool.getMachine(), pool.getName());
			
			if(device == null)
			{
				log.exiting("DPUtils","askForDevicePoolMeasurementGroups (No device)");
				return ret;
			}
			if(device.isAlive() == false)
			{
				log.exiting("DPUtils","askForDevicePoolMeasurementGroups (Not alive)");
				return ret;
			}
			
			DeviceAttribute attribute = device.read_attribute(POOL_ATTR_MEASUREMENT_GROUP_LIST);			
			String[] mgData = attribute.extractStringArray();
			
			for(String mgElem : mgData)
			{
				MeasurementGroup mg = getNewMeasurementGroup(pool, mgElem);
				ret.add(mg);
			}
			log.exiting("DPUtils","askForDevicePoolMeasurementGroups",ret);
			return ret;
		} 
		catch (DevFailed e) 
		{
			log.severe("Failed to ask Device Pool " + pool + " for its measurement groups: " + e.getMessage());
			return ret;
		}
	}		
	
	public Device askForDevice(Machine machine, String deviceInstanceName)
	{
		log.entering("DPUtils", "askForDevice", deviceInstanceName);
		if( deviceInstanceName == null || deviceInstanceName.equals("") )
		{
			log.exiting("DPUtils", "askForDevice (no device name)");
			return null;
		}
		
		try
		{
			Device ret = DeviceFactory.getInstance().getDevice(deviceInstanceName);
			log.exiting("DPUtils", "askForDevice",ret);
			return ret;
		} 
		catch (ConnectionException e) 
		{
			log.severe("Failed to create device " + deviceInstanceName + ": " + e.getMessage());
			return null;
		}
	}
	
	public MacroServer getNewMacroServer(Machine machine, String deviceName, boolean available)
	{
		log.entering("DPUtils", "getNewMacroServer", deviceName);
		if(deviceName == null || deviceName.equals(""))
		{
			log.exiting("DPUtils", "getNewMacroServer (no device name)");
			return null;
		}
		
		MacroServer macroServer = null;
		
		Device device = askForDevice(machine, deviceName);
		
		String alias = device.getAlias();
		
		//TODO URGENT see problem happening with tango alias "nada" problem 
		//alias = null;
		
		if(alias == null || alias.length() == 0)
			alias = deviceName;
		
		macroServer = new MacroServer(machine, alias);

		macroServer.setDevice(device,false);
		
		macroServer.start();
		
		log.exiting("DPUtils", "getNewMacroServer",macroServer);
		return macroServer;
	}	
	
	public DevicePool getNewDevicePool(Machine machine, String deviceName, boolean available)
	{
		log.entering("DPUtils", "getNewDevicePool", deviceName);
		if(deviceName == null || deviceName.equals(""))
		{
			log.exiting("DPUtils", "getNewDevicePool (no device name)");
			return null;
		}
		
		DevicePool pool = null;
		
		if(available == true)
		{
			Device device = askForDevice(machine, deviceName);
			
			String alias = device.getAlias();
			
			//TODO URGENT see problem happening with tango alias "nada" problem 
			//alias = null;
			
			if(alias == null || alias.length() == 0)
				alias = deviceName;
			
			pool = new DevicePool(machine, alias);

			pool.setDevice(device,false);
		}
		else
		{
			pool = new DevicePool(machine, deviceName);
		}
		
		pool.start();
		
		log.exiting("DPUtils", "getNewDevicePool",pool);
		return pool;
	}

	public Controller getNewController(DevicePool pool, String ctrlDesc)
	{
		log.entering("DPUtils","getNewController",pool);
		Matcher m = CTRL_DESCRIPTION_PATTERN.matcher(ctrlDesc);
		
		if(!m.matches() || m.groupCount() < CTRL_DESCRIPTION_ELEM_COUNT)
		{
			log.warning("Failed to parse controller description: " + ctrlDesc);
			return null;
		}
		
		Device device = pool.getDevice();
		
		if(device == null)
			device = askForDevice(pool.getMachine(), pool.getName());
		
		if(device == null)
		{
			log.exiting("DPUtils","getNewController (No device)");
			return null;
		}
		if(device.isAlive() == false)
		{
			log.exiting("DPUtils","getNewController (Not alive)");
			return null;
		}
		
		//for(int i = 0; i < m.groupCount(); i++)	System.out.println(i + ":: " + m.group(i));
		
		Controller ctrl = new Controller();
		
		ctrl.setName(m.group(CTRL_DESCRIPTION_NAME_INDEX));
		ctrl.setState(CtrlState.Ok);
		ctrl.setPool(pool);
		
		String type = m.group(CTRL_DESCRIPTION_TYPE_INDEX);
		String language = m.group(CTRL_DESCRIPTION_LANGUAGE_INDEX);

		String fileName = m.group(CTRL_DESCRIPTION_LIB_INDEX);
		String className = m.group(CTRL_DESCRIPTION_LIB_CLASS_NAME_INDEX);
		
		ControllerClass ctrlClass = pool.getControllerClassByName(fileName, className);
				
		ctrl.setCtrlClass(ctrlClass);
		
		if(ctrlClass == null)
		{
			log.severe("Unable to find controller class " + className + " for " + 
					   ctrl.getName() + ", from list of controller classes: " + pool.getControllerClasses());
			System.out.println("TH #" + Thread.currentThread().getId());
			for(StackTraceElement elem : Thread.currentThread().getStackTrace())
				System.out.println("\t" + elem.toString());
		}
		else 
		{
			if(!ctrl.getType().toString().equalsIgnoreCase(type))
			{
				log.warning("Controller Type mismatch for " + ctrl.getName() + ". Reported by Controller: " + type + ". Reported by Class: " + ctrl.getType());
			}
			
			ControllerLanguage ctrlLanguage = language.equalsIgnoreCase("cpp") ? ControllerLanguage.CPP : ControllerLanguage.Python;		
			
			if(ctrlLanguage != ctrl.getLanguage())
			{
				log.warning("Controller language mismatch for " + ctrl.getName() + ". Reported by Controller: " + ctrlLanguage + ". Reported by Class: " + ctrl.getLanguage());
			}
		}
		
		// Get the Controller instance information
		if(ctrlClass != null)
		{
			try
			{
				DeviceData args = new DeviceData();
				args.insert(new String[] {
						type,
						fileName, 
						className,
						ctrl.getName()});
				
				DeviceData res = device.executeCommand(DevicePoolUtils.POOL_CMD_GET_CTRL_INFO, args);
				
				String[] resArray = res.extractStringArray();
				
				int start = 4;
				
				int propNb = Integer.parseInt(resArray[start++]);
				
				for(int i = start ; i < (propNb*4) + 2; i++)
				{
					String propName = resArray[i++];
					String propType = resArray[i++];
					String propDescr = resArray[i++];
					String propValue = resArray[i];
					
					PropertyInfo propInfo = ctrlClass.getPropertyInfo(propName);
					PropertyInstance prop = new PropertyInstance(propInfo);
					prop.setValue(propValue);
					
					ctrl.addPropertyInstance(prop);
				}
				
				start += (propNb*4);
				
				if(ctrlClass.getType() == ControllerType.PseudoMotor)
				{
					int m_nb = Integer.parseInt(resArray[start]);
					start += m_nb + 1;
					int pm_nb = Integer.parseInt(resArray[start]);
					start += pm_nb + 1;
				}
				else if(ctrlClass.getType() == ControllerType.PseudoCounter)
				{
					int c_nb = Integer.parseInt(resArray[start]);
					start += c_nb + 1;
				} 				
	
			}
			catch (DevFailed e) {
				log.severe("Failed to get Controller information for " + ctrl.getName());
			}
		}
		log.exiting("DPUtils","getNewController",ctrl);
		return ctrl;
	}
	
	public CommunicationChannel getNewCommunicationChannel(DevicePool pool, String comChannelDesc)
	{
		log.entering("DPUtils","getNewCommunicationChannel",pool);
		Matcher m = COM_CHANNEL_DESCRIPTION_PATTERN.matcher(comChannelDesc);
		
		if(!m.matches() || m.groupCount() < COM_CHANNEL_DESCRIPTION_ELEM_COUNT)
		{
			log.warning("Failed to parse communication channel description: " + comChannelDesc);
			return null;
		}
		
		//for(int i = 0; i < m.groupCount(); i++) System.out.println(i + ":: " + m.group(i));
		String alias = m.group(COM_CHANNEL_DESCRIPTION_NAME_INDEX);
		String deviceName = m.group(COM_CHANNEL_DESCRIPTION_TANGO_NAME_INDEX);
		String[] comChannelData = deviceName.split("/");
		Device device = askForDevice(pool.getMachine(), deviceName);
		
		CommunicationChannel comChannel = new CommunicationChannel(pool.getMachine(), alias);
		comChannel.setDevice(device,false);
		comChannel.setIdInController(Integer.parseInt((comChannelData[2])));
		comChannel.setPool(pool);
		
		Controller ctrl = pool.getController(comChannelData[1]);
		
		if(ctrl == null)
		{
			log.info("Controller " + comChannelData[1] + " not found for communication channel: " + comChannel.getName() + 
					", from list of controllers: " + pool.getControllers());
		}
		else
		{
			comChannel.setController(ctrl);
			ctrl.addElement(comChannel);
		}
		comChannel.start();
		log.exiting("DPUtils","getNewCommunicationChannel",comChannel);
		return comChannel;
	}
	
	public Motor getNewMotor(DevicePool pool, String motorDesc)
	{
		log.entering("DPUtils","getNewMotor",pool);
		Matcher m = MOTOR_DESCRIPTION_PATTERN.matcher(motorDesc);
		
		if(!m.matches() || m.groupCount() < MOTOR_DESCRIPTION_ELEM_COUNT)
		{
			log.warning("Failed to parse motor description: " + motorDesc);
			return null;
		}
		
		//for(int i = 0; i < m.groupCount(); i++) System.out.println(i + ":: " + m.group(i));
		String alias = m.group(MOTOR_DESCRIPTION_NAME_INDEX);
		String deviceName = m.group(MOTOR_DESCRIPTION_TANGO_NAME_INDEX);
		String[] motorData = deviceName.split("/");
		Device device = askForDevice(pool.getMachine(), deviceName);
		
		Motor motor = new Motor(pool.getMachine(), alias);
		motor.setDevice(device,false);
		motor.setIdInController(Integer.parseInt((motorData[2])));
		motor.setPool(pool);
		
		Controller ctrl = pool.getController(motorData[1]);
		
		if(ctrl == null)
		{
			log.info("Controller " + motorData[1] + " not found for motor: " + motor.getName() + 
					", from list of controllers: " + pool.getControllers());
		}
		else
		{
			motor.setController(ctrl);
			ctrl.addElement(motor);
		}
		motor.start();
		log.exiting("DPUtils","getNewMotor",motor);
		return motor;
	}
	
	public PseudoMotor getNewPseudoMotor(DevicePool pool, String motorDesc)
	{
		log.entering("DPUtils","getNewPseudoMotor",pool);
		Matcher m = PSEUDO_MOTOR_DESCRIPTION_PATTERN.matcher(motorDesc);
		
		if(!m.matches() || m.groupCount() < PSEUDO_MOTOR_DESCRIPTION_ELEM_COUNT)
		{
			log.warning("Failed to parse pseudo motor description: " + motorDesc);
			return null;
		}
		
		//for(int i = 0; i < m.groupCount(); i++) System.out.println(i + ":: " + m.group(i));
		
		String alias = m.group(PSEUDO_MOTOR_DESCRIPTION_NAME_INDEX);
		String deviceName = m.group(PSEUDO_MOTOR_DESCRIPTION_TANGO_NAME_INDEX);
		String[] motorData = deviceName.split("/");
		Device device = askForDevice(pool.getMachine(), deviceName);
		
		PseudoMotor pseudoMotor = new PseudoMotor(pool.getMachine(), alias);
		pseudoMotor.setDevice(device,false);
		pseudoMotor.setIdInController(Integer.parseInt((motorData[2])));
		pseudoMotor.setPool(pool);
		
		Controller ctrl = pool.getController(motorData[1]);
		
		if(ctrl == null)
		{
			log.info("Controller " + motorData[1] + " not found for pseudo motor: " + pseudoMotor.getName() + 
					", from list of controllers: " + pool.getControllers());
		}
		else
		{
			pseudoMotor.setController(ctrl);
			ctrl.addElement(pseudoMotor);
		}
		
		String motorList =  m.group(PSEUDO_MOTOR_DESCRIPTION_MOTORS_INDEX);
		
		String[] motors = motorList.split(",");
		
		for(String motor : motors)
		{
			motor = motor.trim();
			
			Motor motorElem = pool.getMotor(motor);
			
			if(motorElem == null)
			{
				log.info(pseudoMotor.getName() + " (pseudoMotor) has an unknown motor - " + motor);
				continue;
			}
			pseudoMotor.addMotor(motorElem);
		}		
		
		pseudoMotor.start();
		log.exiting("DPUtils","getNewPseudoMotor",pseudoMotor);
		return pseudoMotor;
	}	
	
	public PseudoCounter getNewPseudoCounter(DevicePool pool, String counterDesc)
	{
		log.entering("DPUtils","getNewPseudoCounter",pool);
		Matcher m = PSEUDO_COUNTER_DESCRIPTION_PATTERN.matcher(counterDesc);
		
		if(!m.matches() || m.groupCount() < PSEUDO_COUNTER_DESCRIPTION_ELEM_COUNT)
		{
			log.warning("Failed to parse pseudo counter description: " + counterDesc);
			return null;
		}
		
		//for(int i = 0; i < m.groupCount(); i++) System.out.println(i + ":: " + m.group(i));
		
		String alias = m.group(PSEUDO_COUNTER_DESCRIPTION_NAME_INDEX);
		String deviceName = m.group(PSEUDO_COUNTER_DESCRIPTION_TANGO_NAME_INDEX);
		String[] counterData = deviceName.split("/");
		Device device = askForDevice(pool.getMachine(), deviceName);
		
		PseudoCounter pseudoCounter = new PseudoCounter(pool.getMachine(), alias);
		pseudoCounter.setDevice(device,false);
		pseudoCounter.setIdInController(Integer.parseInt((counterData[2])));
		pseudoCounter.setPool(pool);
		
		Controller ctrl = pool.getController(counterData[1]);
		
		if(ctrl == null)
		{
			log.info("Controller " + counterData[1] + " not found for pseudo counter: " + pseudoCounter.getName() + 
					", from list of controllers: " + pool.getControllers());
		}
		else
		{
			pseudoCounter.setController(ctrl);
			ctrl.addElement(pseudoCounter);
		}
		
		String counterList =  m.group(PSEUDO_COUNTER_DESCRIPTION_COUNTERS_INDEX);

		if(counterList == null || counterList.length() == 0)
		{
			pseudoCounter.start();
			log.exiting("DPUtils","getNewPseudoCounter",pseudoCounter);
			return pseudoCounter;
		}
		
		String[] counters = counterList.split(",");
		
		for(String counter : counters)
		{
			counter = counter.trim();
			
			ExperimentChannel counterElem = pool.getExperimentChannel(counter);
			
			if(counterElem == null)
			{
				log.info(pseudoCounter.getName() + " (pseudo counter) has an unknown counter - " + counter);
				continue;
			}
			pseudoCounter.addCounter(counterElem);
		}		
		
		pseudoCounter.start();
		log.exiting("DPUtils","getNewPseudoCounter",pseudoCounter);
		return pseudoCounter;
	}	
	
	public MotorGroup getNewEmptyMotorGroup(DevicePool pool, String motorDesc)
	{
		log.entering("DPUtils","getNewEmptyMotorGroup",pool);
		Matcher m = MOTOR_GROUP_DESCRIPTION_PATTERN.matcher(motorDesc);
		
		if(!m.matches() || m.groupCount() < MOTOR_GROUP_DESCRIPTION_ELEM_COUNT)
		{
			log.warning("Failed to parse motorGroup group description: " + motorDesc);
			return null;
		}

		//for(int i = 0; i < m.groupCount(); i++)	System.out.println(i + ":: " + m.group(i));

		String alias = m.group(MOTOR_GROUP_DESCRIPTION_NAME_INDEX);
		String deviceName = m.group(MOTOR_GROUP_DESCRIPTION_TANGO_NAME_INDEX);
		Device device = askForDevice(pool.getMachine(), deviceName);
		
		MotorGroup motorGroup = new MotorGroup(pool.getMachine(), alias);
		motorGroup.setDevice(device,false);
		motorGroup.setPool(pool);
		motorGroup.start();
		log.exiting("DPUtils","getNewEmptyMotorGroup",motorGroup);
		return motorGroup;
	}
	
	/**
	 * Fills the motor group object with data.
	 * Assumes that the motor group already contains valid alias, device, pool
	 * attributes.
	 * 
	 * @param motorGroup
	 * @param motorDesc
	 * @param mgs 
	 * @return
	 */
	public void fillEmptyMotorGroup(MotorGroup motorGroup, String motorDesc, ArrayList<MotorGroup> mgs)
	{
		log.entering("DPUtils","fillEmptyMotorGroup", motorGroup);
		Matcher m = MOTOR_GROUP_DESCRIPTION_PATTERN.matcher(motorDesc);
		
		if(!m.matches() || m.groupCount() < MOTOR_GROUP_DESCRIPTION_ELEM_COUNT)
		{
			log.warning("Failed to parse motorGroup group description: " + motorDesc);
			return;
		}

		//for(int i = 0; i < m.groupCount(); i++)	System.out.println(i + ":: " + m.group(i));
		DevicePool pool = motorGroup.getPool();
		
		String elementList =  m.group(MOTOR_GROUP_DESCRIPTION_MOTORS_INDEX);
		
		String[] elements = elementList.split(",");
		
		for(String element : elements)
		{
			element = element.trim();
			
			SardanaDevice motorElem = pool.getMotor(element);
			
			if(motorElem == null)
			{
				for(MotorGroup motorGroupElem : mgs)
					if(motorGroupElem.getName().equalsIgnoreCase(element))
						motorElem = motorGroupElem;
		
				if(motorElem == null)
				{
					motorElem = pool.getPseudoMotor(element);
				
					if(motorElem == null)
					{
						log.warning(motorGroup.getName() + " (MotorGroup) has an unknown element - " + element);
						continue;
					}
				}
			}
			
			if(motorElem != null)
				motorGroup.addGenericElement(motorElem);
		}
		log.exiting("DPUtils","fillEmptyMotorGroup");
	}
	
	public MotorGroup getNewMotorGroup(DevicePool pool, String motorDesc) 
	{
		log.entering("DPUtils","getNewMotorGroup",pool);
		Matcher m = MOTOR_GROUP_DESCRIPTION_PATTERN.matcher(motorDesc);
		
		if(!m.matches() || m.groupCount() < MOTOR_GROUP_DESCRIPTION_ELEM_COUNT)
		{
			log.warning("Failed to parse motorGroup group description: " + motorDesc);
			return null;
		}

		//for(int i = 0; i < m.groupCount(); i++)	System.out.println(i + ":: " + m.group(i));

		String alias = m.group(MOTOR_GROUP_DESCRIPTION_NAME_INDEX);
		String deviceName = m.group(MOTOR_GROUP_DESCRIPTION_TANGO_NAME_INDEX);
		Device device = askForDevice(pool.getMachine(), deviceName);
		
		MotorGroup motorGroup = new MotorGroup(pool.getMachine(), alias);
		motorGroup.setDevice(device,false);
		motorGroup.setPool(pool);
		
		String elementList =  m.group(MOTOR_GROUP_DESCRIPTION_MOTORS_INDEX);
		
		String[] elements = elementList.split(",");
		
		for(String element : elements)
		{
			element = element.trim();
			
			SardanaDevice motorElem = pool.getMotor(element);
			
			if(motorElem == null)
			{
				motorElem = pool.getMotorGroup(element);
		
				if(motorElem == null)
				{
					motorElem = pool.getPseudoMotor(element);
				
					if(motorElem == null)
					{
						log.info(motorGroup.getName() + " (MotorGroup) has an unknown element - " + element + ". From list of motors:" + pool.getMotors());
						continue;
					}
					motorGroup.addPseudoMotor((PseudoMotor)motorElem);
				}
				else 
				{
					motorGroup.addMotorGroup((MotorGroup)motorElem);
				}
			}
			else 
			{
				motorGroup.addMotor((Motor)motorElem);
			}
		}
		motorGroup.start();
		log.exiting("DPUtils","getNewMotorGroup",motorGroup);
		return motorGroup;
	}
	
	public MeasurementGroup getNewMeasurementGroup(DevicePool pool, String channelDesc) 
	{
		log.entering("DPUtils","getNewMeasurementGroup",pool);
		Matcher m = MEASUREMENT_GROUP_DESCRIPTION_PATTERN.matcher(channelDesc);
		
		if(!m.matches() || m.groupCount() < MEASUREMENT_GROUP_DESCRIPTION_ELEM_COUNT)
		{
			log.warning("Failed to parse measurement group description: " + channelDesc);
			return null;
		}

		//for(int i = 0; i < m.groupCount(); i++)	System.out.println(i + ":: " + m.group(i));

		String alias = m.group(MEASUREMENT_GROUP_DESCRIPTION_NAME_INDEX);
		String deviceName = m.group(MEASUREMENT_GROUP_DESCRIPTION_TANGO_NAME_INDEX);
		Device device = askForDevice(pool.getMachine(), deviceName);
		
		MeasurementGroup measurementGroup = new MeasurementGroup(pool.getMachine(), alias);
		measurementGroup.setDevice(device,false);
		measurementGroup.setPool(pool);
		
		String channelList =  m.group(MEASUREMENT_GROUP_DESCRIPTION_CHANNELS_INDEX);
		
		if(channelList == null || channelList.length() == 0)
		{
			log.warning("Mntgrp " + alias + " has 0 channels");
		}
		
		String[] channels = channelList == null || channelList.length() == 0 ?
			new String[0] : 
			channelList.split(",");
		
		for(String channel : channels)
		{
			channel = channel.trim();
			
			ExperimentChannel channelElem = pool.getExperimentChannel(channel);
			
			if(channelElem == null)
			{
				log.info(measurementGroup.getName() + "Failed to find channel " + channel + 
						" for measurement group:"  + measurementGroup.getName() + 
						", from a list of channels: " + pool.getExperimentChannels());
				continue;
			}
			else 
			{
				measurementGroup.addGenericChannel(channelElem);
			}
		}
		measurementGroup.start();
		log.exiting("DPUtils","getNewMeasurementGroup",measurementGroup);
		return measurementGroup;
	}	

	public ExperimentChannel getNewExperimentChannel(DevicePool pool, String channelDesc)
	{
		log.entering("DPUtils","getNewExperimentChannel",pool);
		Matcher m = EXP_CHANNEL_DESCRIPTION_PATTERN.matcher(channelDesc);
		
		if(!m.matches() || m.groupCount() < EXP_CHANNEL_DESCRIPTION_ELEM_COUNT)
		{
			log.warning("Failed to parse experimentChannel description: " + channelDesc);
			return null;
		}
		
		String alias = m.group(EXP_CHANNEL_DESCRIPTION_NAME_INDEX);
		String deviceName = m.group(EXP_CHANNEL_DESCRIPTION_TANGO_NAME_INDEX);
		String channelType = m.group(EXP_CHANNEL_DESCRIPTION_TYPE_INDEX);
		String[] channelData = deviceName.split("/");
		Device device = askForDevice(pool.getMachine(), deviceName);
		
		ExperimentChannel channel = null;
		
		if(channelType.startsWith("Counter"))
			channel = new CounterTimer(pool.getMachine(), alias);
		else if(channelType.startsWith("Zero"))
			channel = new ZeroDExpChannel(pool.getMachine(), alias);
		else if(channelType.startsWith("PseudoCounter"))
			channel = new PseudoCounter(pool.getMachine(), alias);
		
		if(channel != null)
		{
			channel.setPool(pool);
			channel.setDevice(device,false);
			channel.setIdInController(Integer.parseInt((channelData[2])));
	
			Controller ctrl = pool.getController(channelData[1]);
			
			if(ctrl == null)
			{
				log.info("Controller " + channelData[1] + " not found for motor: "
						 + channel.getName() + ",from a list of controllers:" + pool.getControllers() );
			}
			else
			{
				channel.setController(ctrl);
				ctrl.addElement(channel);
			}
		}
		else
		{
			log.severe("Unable to identify channel type for " + alias );
			return null;
		}
		channel.start();
		log.exiting("DPUtils","getNewExperimentChannel",channel);
		return channel;
	}
	
	public String[] getControllerNames(String[] data)
	{
		String[] ret = new String[data.length];
		
		int i = 0;
		for(String ctrlDesc : data)
		{
			Matcher m = CTRL_DESCRIPTION_PATTERN.matcher(ctrlDesc);
			
			if(!m.matches() || m.groupCount() < CTRL_DESCRIPTION_ELEM_COUNT)
			{
				ret[i] = "!Unknown!";
			}
			else
			{
				ret[i] = m.group(CTRL_DESCRIPTION_NAME_INDEX);
			}
			i++;
		}
		return ret;
	}
	
	public String[] getMotorNames(String[] data)
	{
		String[] ret = new String[data.length];
		
		int i = 0;
		for(String motorDesc : data)
		{
			Matcher m = MOTOR_DESCRIPTION_PATTERN.matcher(motorDesc);
			
			if(!m.matches() || m.groupCount() < MOTOR_DESCRIPTION_ELEM_COUNT)
			{
				ret[i] = "!Unknown!";
			}
			else
			{
				ret[i] = m.group(MOTOR_DESCRIPTION_NAME_INDEX);
			}
			i++;
		}
		return ret;
	}
	
	public String[] getMotorGroupNames(String[] data)
	{
		String[] ret = new String[data.length];
		
		int i = 0;
		for(String motorGroupDesc : data)
		{
			Matcher m = MOTOR_GROUP_DESCRIPTION_PATTERN.matcher(motorGroupDesc);
			
			if(!m.matches() || m.groupCount() < MOTOR_GROUP_DESCRIPTION_ELEM_COUNT)
			{
				ret[i] = "!Unknown!";
			}
			else
			{
				ret[i] = m.group(MOTOR_GROUP_DESCRIPTION_NAME_INDEX);
			}
			i++;
		}
		return ret;
	}
	
	public String[] getMeasurementGroupNames(String[] data)
	{
		String[] ret = new String[data.length];
		
		int i = 0;
		for(String measurementGroupDesc : data)
		{
			Matcher m = MEASUREMENT_GROUP_DESCRIPTION_PATTERN.matcher(measurementGroupDesc);
			
			if(!m.matches() || m.groupCount() < MEASUREMENT_GROUP_DESCRIPTION_ELEM_COUNT)
			{
				ret[i] = "!Unknown!";
			}
			else
			{
				ret[i] = m.group(MEASUREMENT_GROUP_DESCRIPTION_NAME_INDEX);
			}
			i++;
		}
		return ret;
	}	

	public String[] getPseudoMotorNames(String[] data)
	{
		String[] ret = new String[data.length];
		
		int i = 0;
		for(String pseudoMotorDesc : data)
		{
			Matcher m = PSEUDO_MOTOR_DESCRIPTION_PATTERN.matcher(pseudoMotorDesc);
			
			if(!m.matches() || m.groupCount() < PSEUDO_MOTOR_DESCRIPTION_ELEM_COUNT)
			{
				ret[i] = "!Unknown!";
			}
			else
			{
				ret[i] = m.group(PSEUDO_MOTOR_DESCRIPTION_NAME_INDEX);
			}
			i++;
		}
		return ret;
	}

	public String[] getPseudoCounterNames(String[] data)
	{
		String[] ret = new String[data.length];
		
		int i = 0;
		for(String pseudoCounterDesc : data)
		{
			Matcher m = PSEUDO_COUNTER_DESCRIPTION_PATTERN.matcher(pseudoCounterDesc);
			
			if(!m.matches() || m.groupCount() < PSEUDO_COUNTER_DESCRIPTION_ELEM_COUNT)
			{
				ret[i] = "!Unknown!";
			}
			else
			{
				ret[i] = m.group(PSEUDO_COUNTER_DESCRIPTION_NAME_INDEX);
			}
			i++;
		}
		return ret;
	}

	
	public String[] getExperimentChannelNames(String[] data)
	{
		String[] ret = new String[data.length];
		
		int i = 0;
		for(String channelDesc : data)
		{
			Matcher m = EXP_CHANNEL_DESCRIPTION_PATTERN.matcher(channelDesc);
			
			if(!m.matches() || m.groupCount() < EXP_CHANNEL_DESCRIPTION_ELEM_COUNT)
			{
				ret[i] = "!Unknown!";
			}
			else
			{
				ret[i] = m.group(EXP_CHANNEL_DESCRIPTION_NAME_INDEX);
			}
			i++;
		}
		return ret;
	}

	public String[] getCommunicationChannelNames(String[] data)
	{
		String[] ret = new String[data.length];
		
		int i = 0;
		for(String channelDesc : data)
		{
			Matcher m = COM_CHANNEL_DESCRIPTION_PATTERN.matcher(channelDesc);
			
			if(!m.matches() || m.groupCount() < COM_CHANNEL_DESCRIPTION_ELEM_COUNT)
			{
				ret[i] = "!Unknown!";
			}
			else
			{
				ret[i] = m.group(COM_CHANNEL_DESCRIPTION_NAME_INDEX);
			}
			i++;
		}
		return ret;
	}
	
	public static void clearDevicePool(DevicePool devicePool) 
	{
		for(PseudoMotor pm : devicePool.getPseudoMotors())
		{
			DeviceData data;
			try 
			{
				data = new DeviceData();
				data.insert(pm.getName());
				devicePool.getDevice().command_inout("DeletePseudoMotor", data);
			} 
			catch (DevFailed e) 
			{
				log.warning("Failed to delete Pseudo Motor " + pm.getName());
			}
		}
		devicePool.getPseudoMotors().clear();

		for(MotorGroup mg : devicePool.getMotorGroups())
		{
			if(!mg.isInternal())
			{
				DeviceData data;
				try 
				{
					data = new DeviceData();
					data.insert(mg.getName());
					devicePool.getDevice().command_inout("DeleteMotorGroup", data);
				} 
				catch (DevFailed e) 
				{
					log.warning("Failed to delete Motor Group " + mg.getName());
				}
			}
		}
		devicePool.getMotorGroups().clear();
		
		for(Motor motor : devicePool.getMotors())
		{
			DeviceData data;
			try 
			{
				data = new DeviceData();
				data.insert(motor.getName());
				devicePool.getDevice().command_inout("DeleteMotor", data);
			} 
			catch (DevFailed e) 
			{
				log.warning("Failed to delete Motor " + motor.getName());
			}
		}
		devicePool.clearMotors();
		
		for(Controller ctrl : devicePool.getControllers())
		{
			DeviceData data;
			try 
			{
				data = new DeviceData();
				data.insert(ctrl.getName());
				devicePool.getDevice().command_inout("DeleteController", data);
			} 
			catch (DevFailed e) 
			{
				log.warning("Failed to delete Motor " + ctrl.getName());
			}
		}
		devicePool.clearControllers();
	}

	/**
	 * Translates the value inside a string into the proper object type
	 * depending on the given property type.
	 * 
	 * @param type
	 * @param value
	 * @return
	 */
	public static Object toPropertyValue(PropertyType type, String value)
	{
		if(value == null)
			return null;
		
		if(type == PropertyType.DevLong) return Integer.valueOf(value);
		if(type == PropertyType.DevBoolean) return Boolean.valueOf(value);
		if(type == PropertyType.DevDouble) return Double.valueOf(value);
		if(type == PropertyType.DevString) return value;

		String[] elems = value.split("\n");
		if(type == PropertyType.DevVarLongArray) 
		{
			Vector<Integer> ret = new Vector<Integer>(elems.length); 
			for(String elem : elems)
				ret.add(Integer.valueOf(elem));
			return ret;
		}
		if(type == PropertyType.DevVarBooleanArray)
		{
			Vector<Boolean> ret = new Vector<Boolean>(elems.length); 
			for(String elem : elems)
				ret.add(Boolean.valueOf(elem));
			return ret;
		}
		if(type == PropertyType.DevVarDoubleArray) 
		{
			Vector<Double> ret = new Vector<Double>(elems.length); 
			for(String elem : elems)
				ret.add(Double.valueOf(elem));
			return ret;			
		}
		if(type == PropertyType.DevVarStringArray) 
		{
			Vector<String> ret = new Vector<String>(elems.length); 
			for(String elem : elems)
				ret.add(elem);
			return ret;
		}
		
		return null;
	}
	
	public static String toPropertyValueString(PropertyType type, Object value)
	{
		if(value == null)
			return null;
		
		if(type.isSimpleType()) 
			return value.toString();

		else if(type == PropertyType.DevVarLongArray) 
		{
			Vector<Integer> vec = (Vector<Integer>) value;
			StringBuffer buff = new StringBuffer();
			for(int i = 0; i < vec.size(); i++)
			{
				buff.append(vec.get(i));
				if(i < vec.size()-1) buff.append("\n");
			}
			return buff.toString();
		}
		else if(type == PropertyType.DevVarBooleanArray)
		{
			Vector<Boolean> vec = (Vector<Boolean>)value;
			StringBuffer buff = new StringBuffer();
			for(int i = 0; i < vec.size(); i++)
			{
				buff.append(vec.get(i));
				if(i < vec.size()-1) buff.append("\n");
			}
			return buff.toString();
		}
		else if(type == PropertyType.DevVarDoubleArray) 
		{
			Vector<Double> vec = (Vector<Double>)value;
			StringBuffer buff = new StringBuffer();
			for(int i = 0; i < vec.size(); i++)
			{
				buff.append(vec.get(i));
				if(i < vec.size()-1) buff.append("\n");
			}
			return buff.toString();			
		}
		else if(type == PropertyType.DevVarStringArray) 
		{
			Vector<String> vec = (Vector<String>)value;
			StringBuffer buff = new StringBuffer();
			for(int i = 0; i < vec.size(); i++)
			{
				buff.append(vec.get(i));
				if(i < vec.size()-1) buff.append("\n");
			}
			return buff.toString();	
		}
		else 
			return null;		
	}
	
	public static int getPropertyValueLineCount(PropertyType type, Object value)
	{
		if(value == null)
			return 0;
		
		if(type.isSimpleType()) 
			return 1;
		else
		{
			Vector<?> vec = (Vector<?>) value;
			return vec.size();
		}
	}
	
	public static PropertyType fromTangoTypeToPoolType(AttributeInfoEx info)
	{
		if(info.data_format == AttrDataFormat.SCALAR)
		{
			if(info.data_type == TangoConst.Tango_DEV_BOOLEAN)
				return PropertyType.DevBoolean;
			else if(info.data_type == TangoConst.Tango_DEV_LONG)
				return PropertyType.DevLong;
			else if(info.data_type == TangoConst.Tango_DEV_DOUBLE)
				return PropertyType.DevDouble;
			else if(info.data_type == TangoConst.Tango_DEV_STRING)
				return PropertyType.DevString;
			else
				return PropertyType.InvalidPropertyType;
		}
		else if(info.data_format == AttrDataFormat.SPECTRUM)
		{
			if(info.data_type == TangoConst.Tango_DEV_BOOLEAN)
				return PropertyType.DevVarBooleanArray;
			else if(info.data_type == TangoConst.Tango_DEV_LONG)
				return PropertyType.DevVarLongArray;
			else if(info.data_type == TangoConst.Tango_DEV_DOUBLE)
				return PropertyType.DevVarDoubleArray;
			else if(info.data_type == TangoConst.Tango_DEV_STRING)
				return PropertyType.DevVarStringArray;
			else
				return PropertyType.InvalidPropertyType;
		}
		else
			return PropertyType.InvalidPropertyType;
	}
	
	/**
	 * 
	 * @param s
	 * @return
	 * 0 - attribute/command
	 * 1 - attribute/command name
	 * 2 - polling period (mS)
	 * 3 - ring buffer depth
	 */
	public static String[] extractPollingInfo(String s) 
	{
		String[] splitted = s.split("\n");

		String[] ret = new String[4];
		if (splitted[0].startsWith("Polled attribute"))
			ret[0] = "attribute";
		else
			ret[0] = "command";

		ret[1] = extractEndValue(splitted[0]); // attribute/command name
		ret[2] = extractEndValue(splitted[1]); // polling period
		ret[3] = extractEndValue(splitted[2]); // ring buffer depth

		return ret;
	}	
	
	protected static String extractEndValue(String s) 
	{
		int i = s.lastIndexOf('=');
		if (i != -1)
			return s.substring(i+1, s.length()).trim();
		else
			return "";
	}

	/**
	 * 
	 * @param deviceName
	 * @param field
	 * @return
	 * 0 - logging level
	 * 1..n-1 - logging target
	 */
	public static String[] getCurrentLoggingInfo(SardanaDevice device) 
	{
		String[] result = null;
		String devadmin;

		String deviceName = device.getDeviceName();
		
		try 
		{
			DbDevImportInfo info = device.getDataBase().import_device(deviceName);
			devadmin = "dserver/" + info.server;
			DeviceData argin = new DeviceData();
			DeviceProxy ds = new DeviceProxy(devadmin);
			DeviceData argout;

			argin.insert(deviceName);
			argout = ds.command_inout("GetLoggingTarget",argin);
			String[] res1 = argout.extractStringArray();
			
			String[] names = new String[1];
			names[0] = deviceName;
			argin.insert(names);
			argout = ds.command_inout("GetLoggingLevel",argin);
			DevVarLongStringArray res=argout.extractLongStringArray();
			
			result = new String[res1.length+1];
			
			result[0]=logging_level[res.lvalue[0]];
			
			for(int i = 0; i < res1.length; i++)
				result[i+1] = res1[i];
			
			return result;

		} 
		catch (DevFailed e) {}

		return null;
	}	

	public static String[] getLoggingTarget(SardanaDevice device) 
	{
		String deviceName = device.getDeviceName();
		
		try 
		{
			return device.getDataBase().get_device_property(deviceName, "logging_target").extractStringArray();
		} 
		catch (DevFailed e)	{}

		return null;
	}

	public static String getLoggingLevel(SardanaDevice device) 
	{
		String deviceName = device.getDeviceName();
		
		try 
		{
			String[] ret = device.getDataBase().get_device_property(deviceName, "logging_level").extractStringArray();
			return ret == null || ret.length == 0 ? null : ret[0];
			
		} 
		catch (DevFailed e) {}

		return null;
	}
	
	public static String getLoggingRft(SardanaDevice device) 
	{
		String deviceName = device.getDeviceName();
		try 
		{
			String[] ret = device.getDataBase().get_device_property(deviceName, "logging_rft").extractStringArray();
			return ret == null || ret.length == 0 ? null : ret[0];
		} 
		catch (DevFailed e) {}

		return null;

	}
	
	public void createPool(PoolCreationInfo info) throws DevFailed
	{
		Database db = info.getMachine().getDataBase();
    	
		try
		{
			db.add_device(info.getPooldeviceName(), PoolCreationInfo.serverName,PoolCreationInfo.serverName + "/" + info.getInstanceName());
			
			DbDatum properties[] = new DbDatum[] { new DbDatum("PoolPath",info.getPoolPath()) };
			db.put_device_property(info.getPooldeviceName(), properties);
			
			if(info.getAliasName() != null && info.getAliasName().length() > 0)
			{
				db.put_device_alias(info.getPooldeviceName(),info.getAliasName());
			}
		}
		catch(DevFailed e)
		{
			throw e;
		}
	}
	
	public void createMacroServer(MacroServerCreationInfo info) throws DevFailed
	{
		Database db = info.getMachine().getDataBase();
    	
		try
		{
			String msName = info.getMacroServerDeviceName();
			String msAlias = info.getMacroServerAliasName();
			db.add_device(msName, MacroServerCreationInfo.macroServerServerName,MacroServerCreationInfo.macroServerServerName + "/" + info.getInstanceName());
						
			if(msAlias != null && msAlias.length() > 0)
			{
				db.put_device_alias(msName,msAlias);
			}
			
			DbDatum msProps[] = new DbDatum[] { 
					new DbDatum("MacroPath",info.getMacroPath()),
					new DbDatum("PoolNames",info.getPoolNames())
			};
			db.put_device_property(msName, msProps);
			
			String doorName = info.getDoorDeviceName();
			if(doorName != null && doorName.length() > 0)
			{
				db.add_device(info.getDoorDeviceName(), MacroServerCreationInfo.doorServerName,MacroServerCreationInfo.macroServerServerName + "/" + info.getInstanceName());
				
				String doorAlias = info.getDoorAlias();
				
				if(doorAlias != null && doorAlias.length() > 0)
				{
					db.put_device_alias(info.getDoorDeviceName(),info.getDoorAlias());
				}
			}

			DbDatum doorProps[] = new DbDatum[] { new DbDatum("MacroServerName", msName) };
			db.put_device_property(doorName, doorProps);
			
		}
		catch(DevFailed e)
		{
			throw e;
		}
	}	
	
	public boolean deviceExists(Machine machine,String devName,String serverName,String className) throws DevFailed 
	{
		String[] devList = getDevices(machine, serverName, className);
		for(String deviceName : devList)
			if(deviceName.equalsIgnoreCase(devName))
				return true;
		return false;
	}
	
	public static String devFailedToString(DevFailed e)
	{
		return devFailedToString(e,1);
	}
	
	public static String devFailedToString(DevFailed e, int level)
	{
		StringBuffer buff = new StringBuffer();
		level = Math.min(e.errors.length, level);
		for(int i = 0; i < level;i++)
		{
			buff.append("[(" + i + ") ");
			buff.append(e.errors[i].desc);
			buff.append(", origin: " + e.errors[i].origin);
			buff.append(", reason: " + e.errors[i].reason);
			buff.append("] ");
		}
		return buff.toString();
	}
}
