package es.cells.sardana.client.framework;

import java.awt.event.ActionEvent;
import java.io.File;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Enumeration;
import java.util.List;
import java.util.Vector;
import java.util.logging.Handler;
import java.util.logging.LogManager;
import java.util.logging.Logger;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import javax.swing.AbstractAction;
import javax.swing.JFileChooser;
import javax.swing.JMenu;
import javax.swing.JMenuBar;
import javax.swing.JMenuItem;
import javax.swing.JOptionPane;
import javax.swing.JSeparator;
import javax.swing.SwingUtilities;
import javax.swing.filechooser.FileFilter;
import javax.swing.tree.TreePath;

import org.apache.xmlbeans.XmlException;

import es.cells.sardana.client.framework.config.PoolsDocument;
import es.cells.sardana.client.framework.gui.IImageResource;
import es.cells.sardana.client.framework.gui.atk.widget.tree.ComChannelsTreeNode;
import es.cells.sardana.client.framework.gui.atk.widget.tree.ControllersTreeNode;
import es.cells.sardana.client.framework.gui.atk.widget.tree.DevicePoolTreeNode;
import es.cells.sardana.client.framework.gui.atk.widget.tree.DoorTreeNode;
import es.cells.sardana.client.framework.gui.atk.widget.tree.ElementListTreeNode;
import es.cells.sardana.client.framework.gui.atk.widget.tree.ExpChannelsTreeNode;
import es.cells.sardana.client.framework.gui.atk.widget.tree.MachineTreeNode;
import es.cells.sardana.client.framework.gui.atk.widget.tree.MacroServerTreeNode;
import es.cells.sardana.client.framework.gui.atk.widget.tree.MeasurementGroupsTreeNode;
import es.cells.sardana.client.framework.gui.atk.widget.tree.MotorGroupsTreeNode;
import es.cells.sardana.client.framework.gui.atk.widget.tree.MotorsTreeNode;
import es.cells.sardana.client.framework.gui.atk.widget.tree.SardanaRootTreeNode;
import es.cells.sardana.client.framework.gui.atk.widget.tree.SardanaTreeNode;
import es.cells.sardana.client.framework.gui.dialog.AddMacroServerDialog;
import es.cells.sardana.client.framework.gui.dialog.AddPoolDialog;
import es.cells.sardana.client.framework.gui.dialog.SardanaConfigEditor;
import es.cells.sardana.client.framework.gui.dialog.SardanaConfigView;
import es.cells.sardana.client.framework.gui.panel.DisplayPanel;
import es.cells.sardana.client.framework.gui.panel.TreePanel;
import es.cells.sardana.client.framework.macroserver.Door;
import es.cells.sardana.client.framework.macroserver.MacroServer;
import es.cells.sardana.client.framework.pool.CommunicationChannel;
import es.cells.sardana.client.framework.pool.Controller;
import es.cells.sardana.client.framework.pool.CtrlState;
import es.cells.sardana.client.framework.pool.DevicePool;
import es.cells.sardana.client.framework.pool.DevicePoolUtils;
import es.cells.sardana.client.framework.pool.ExperimentChannel;
import es.cells.sardana.client.framework.pool.Machine;
import es.cells.sardana.client.framework.pool.MeasurementGroup;
import es.cells.sardana.client.framework.pool.Motor;
import es.cells.sardana.client.framework.pool.MotorGroup;
import es.cells.sardana.client.framework.pool.PseudoMotor;
import es.cells.sardana.client.framework.pool.SardanaDevice;
import es.cells.sardana.client.framework.pool.event.PoolInternalStringSpectrumEvent;
import es.cells.sardana.client.gui.swing.SwingResource;
import fr.esrf.tangoatk.core.AttributeStateEvent;
import fr.esrf.tangoatk.core.DevStateScalarEvent;
import fr.esrf.tangoatk.core.Device;
import fr.esrf.tangoatk.core.ErrorEvent;
import fr.esrf.tangoatk.core.IDevStateScalarListener;
import fr.esrf.tangoatk.core.IStringScalarListener;
import fr.esrf.tangoatk.core.IStringSpectrumListener;
import fr.esrf.tangoatk.core.StringScalarEvent;
import fr.esrf.tangoatk.core.StringSpectrumEvent;
import fr.esrf.tangoatk.core.attribute.DevStateScalar;
import fr.esrf.tangoatk.core.attribute.StringScalar;
import fr.esrf.tangoatk.core.attribute.StringSpectrum;

public class SardanaManager {
	
	public static boolean EXTENDED_LAF = true; 
	
	protected static SardanaManager instance = null;
	
	protected Logger log;
	
	protected IPreferences loadedPreferences;
	protected Preferences currentPreferences;
	
	protected TreePanel leftPanel = null;
	protected DisplayPanel rightPanel = null;

	protected SardanaDataModel sardanaModel;
	
	protected DevicePoolUtils utils;
	
	protected ArrayList<Handler> logHandlers = new ArrayList<Handler>();
	
	protected StateListener stateListener;
	protected DevicePoolListener poolListener;
	protected MacroServerListener macroServerListener;
	protected ControllerListListener controllerListListener;
	protected MotorListListener motorListListener;
	protected MotorGroupListListener motorGroupListListener;
	protected PseudoMotorListListener pseudoMotorListListener;
	protected ExpChannelListListener expChannelListListener;
	protected MeasurementGroupListListener measurementGroupListListener;
	protected ComChannelListListener comChannelListListener;
	
	protected JMenuBar menuBar;
	
	protected SardanaManager(IPreferences preferences)
	{
		this.loadedPreferences = preferences;
		this.currentPreferences = new Preferences(loadedPreferences);
	}
	
	public void init(Handler handler) 
	{	
		logHandlers = new ArrayList<Handler>();
		
		if(handler != null)
			logHandlers.add(handler);
		
		log = getLogger(this.getClass().getName());
		
		utils = DevicePoolUtils.getInstance();
		
		createSardanaDataModel();
	}

	public static SardanaManager getInstance()
	{
		return getInstance(null);
	}
	
	public static SardanaManager getInstance(String[] args)
	{
		if(instance == null)
		{
			IPreferences prefs = null;
			if(args == null || args.length == 0)
				prefs = new DefaultPreferences();
			else
				prefs = new CmdLinePreferences(args);
				
			instance = new SardanaManager(prefs);
		}
		return instance;
	}
	
	public void addHandler(Handler handler)
	{
		Enumeration<String> logs = LogManager.getLogManager().getLoggerNames();
		
		while(logs.hasMoreElements())
		{
			String log = logs.nextElement();
			
			Logger logger = LogManager.getLogManager().getLogger(log);
			
			logger.addHandler(handler);
		}
	}
	
	public Handler getDefaultLogHandler()
	{
		if(logHandlers.size() > 0)
			return logHandlers.get(0);
		return null;
	}
	
	public Logger getLogger(String name)
	{
		Logger logger = Logger.getLogger(name);
		
		for(Handler handler : logHandlers)
		{
			logger.addHandler(handler);
		}
		return logger;
	}

	public Preferences getCurrentPreferences()
	{
		return currentPreferences;
	}
	
	public IPreferences getLoadedPreferences() 
	{
		return loadedPreferences;
	}

	public TreePanel getLeftPanel() 
	{
		if(leftPanel == null)
		{
			leftPanel = new TreePanel( getRightPanel() );
		}
		return leftPanel;
	}

	public DisplayPanel getRightPanel() 
	{
		if(rightPanel == null)
			rightPanel = new DisplayPanel();
		return rightPanel;
	}
	
	public SardanaDataModel getDataModel()
	{
		return sardanaModel;
	}
	
	protected SardanaRootTreeNode getRootNode()
	{
		SardanaDataModel model = getDataModel();
		
		if(model == null)
			return null;
		
		return (SardanaRootTreeNode) getDataModel().getRoot();
	}
	
	public MachineTreeNode getMachineNode()
	{
		//TODO change in the future to support different machines
		
		SardanaRootTreeNode rootNode = getRootNode();
		
		if(rootNode == null)
			return null;
		
		if(rootNode.getChildCount() == 0)
			return null;
		
		return (MachineTreeNode) rootNode.getChildAt(0);
	}

	public SardanaDevice getSardanaDevice(String deviceName)
	{
		CommunicationChannel comChannel = getComChannelByDeviceName(deviceName);
		if(comChannel != null)
		{
			return comChannel;
		}
		
		Motor motor = getMotorByDeviceName(deviceName);
		if(motor != null)
		{
			return motor;
		}
		
		MotorGroup motorGroup = getMotorGroupByDeviceName(deviceName);
		if(motorGroup != null)
		{
			return motorGroup;
		}
		
		PseudoMotor pseudoMotor = getPseudoMotorByDeviceName(deviceName);
		if(pseudoMotor != null)
		{
			return pseudoMotor;
		}
		
		ExperimentChannel channel = getExpChannelByDeviceName(deviceName);
		if(channel != null)
		{
			return channel;
		}
		
		MeasurementGroup measurementGroup = getMeasurementGroupByDeviceName(deviceName);
		if(measurementGroup != null)
		{
			return measurementGroup;
		}		
		return null;
	}
	
	public Device getDevice(String deviceName)
	{
		SardanaDevice sd = getSardanaDevice(deviceName);
		
		return (sd == null) ? null : sd.getDevice();
	}

	public SardanaTreeNode getSardanaNodeByDeviceName(String deviceName)
	{
		SardanaTreeNode node = getMotorNodeByDeviceName(deviceName);
		if(node != null)
		{
			return node;
		}
		
		node = getMotorGroupNodeByDeviceName(deviceName);
		if(node != null)
		{
			return node;
		}
		
		node = getExpChannelNodeByDeviceName(deviceName);
		if(node != null)
		{
			return node;
		}
		
		node = getMeasurementGroupNodeByDeviceName(deviceName);
		if(node != null)
		{
			return node;
		}
				
		return null;
	}	
	
	public SardanaTreeNode getControllerNodeByName(String name)
	{
		for(DevicePoolTreeNode deviceNode : getPoolNodes())
		{
			ControllersTreeNode ctrlsNode = deviceNode.getControllersNode();
			
			if(ctrlsNode == null)
				continue;
			
			SardanaTreeNode node = ctrlsNode.getChildByDeviceName(name);
			
			if(node != null)
				return node;
		}
		return null;
	}

	public SardanaTreeNode getComChannelNodeByDeviceName(String deviceName)
	{
		for(DevicePoolTreeNode deviceNode : getPoolNodes())
		{
			ComChannelsTreeNode comChannelsNode = deviceNode.getComChannelsNode();
			
			if(comChannelsNode == null)
				continue;
			
			SardanaTreeNode node = comChannelsNode.getChildByDeviceName(deviceName);
			
			if(node != null)
				return node;
		}
		return null;
	}
	
	public SardanaTreeNode getMotorNodeByDeviceName(String deviceName)
	{
		for(DevicePoolTreeNode deviceNode : getPoolNodes())
		{
			MotorsTreeNode motorsNode = deviceNode.getMotorsNode();
			
			if(motorsNode == null)
				continue;
			
			SardanaTreeNode node = motorsNode.getChildByDeviceName(deviceName);
			
			if(node != null)
				return node;
		}
		return null;
	}

	public SardanaTreeNode getMotorGroupNodeByDeviceName(String deviceName)
	{
		for(DevicePoolTreeNode deviceNode : getPoolNodes())
		{
			MotorGroupsTreeNode motorGroupsNode = deviceNode.getMotorGroupsNode();
			
			if(motorGroupsNode == null)
				continue;
			
			SardanaTreeNode node = motorGroupsNode.getChildByDeviceName(deviceName);
			
			if(node != null)
				return node;
		}
		return null;
	}

	public SardanaTreeNode getExpChannelNodeByDeviceName(String deviceName)
	{
		for(DevicePoolTreeNode deviceNode : getPoolNodes())
		{
			ExpChannelsTreeNode channelsNode = deviceNode.getExpChannelsNode();
			
			if(channelsNode == null)
				continue;			
			
			SardanaTreeNode node = channelsNode.getChildByDeviceName(deviceName);
			
			if(node != null)
				return node;
		}
		return null;
	}
	
	public SardanaTreeNode getMeasurementGroupNodeByDeviceName(String deviceName)
	{
		for(DevicePoolTreeNode deviceNode : getPoolNodes())
		{
			MeasurementGroupsTreeNode measurementGroupsNode = deviceNode.getMeasurementGroupsNode();
			
			if(measurementGroupsNode == null)
				continue;
			
			SardanaTreeNode node = measurementGroupsNode.getChildByDeviceName(deviceName);
			
			if(node != null)
				return node;
		}
		return null;
	}	
	
	public CommunicationChannel getComChannelByDeviceName(String deviceName)
	{
		for(DevicePoolTreeNode deviceNode : getPoolNodes())
		{
			DevicePool pool = deviceNode.getDevicePool();
			
			CommunicationChannel channel = pool.getCommunicationChannelByDeviceName(deviceName);
			
			if(channel != null)
				return channel;
		}
		return null;
	}	
	
	public Motor getMotorByDeviceName(String deviceName)
	{
		for(DevicePoolTreeNode deviceNode : getPoolNodes())
		{
			DevicePool pool = deviceNode.getDevicePool();
			
			Motor motor = pool.getMotorByDeviceName(deviceName);
			
			if(motor != null)
				return motor;
		}
		return null;
	}
	
	public MotorGroup getMotorGroupByDeviceName(String deviceName)
	{
		for(DevicePoolTreeNode deviceNode : getPoolNodes())
		{
			DevicePool pool = deviceNode.getDevicePool();
			
			MotorGroup motorGroup = pool.getMotorGroupByDeviceName(deviceName);
			
			if(motorGroup != null)
				return motorGroup;
		}
		return null;
	}	

	public PseudoMotor getPseudoMotorByDeviceName(String deviceName)
	{
		for(DevicePoolTreeNode deviceNode : getPoolNodes())
		{
			DevicePool pool = deviceNode.getDevicePool();
			
			PseudoMotor pseudoMotor = pool.getPseudoMotorByDeviceName(deviceName);
			
			if(pseudoMotor != null)
				return pseudoMotor;
		}
		return null;
	}
	
	public ExperimentChannel getExpChannelByDeviceName(String deviceName)
	{
		for(DevicePoolTreeNode deviceNode : getPoolNodes())
		{
			DevicePool pool = deviceNode.getDevicePool();
			
			ExperimentChannel channel = pool.getExperimentChannelByDeviceName(deviceName);
			
			if(channel != null)
				return channel;
		}
		return null;
	}
	
	public MeasurementGroup getMeasurementGroupByDeviceName(String deviceName)
	{
		for(DevicePoolTreeNode deviceNode : getPoolNodes())
		{
			DevicePool pool = deviceNode.getDevicePool();
			
			MeasurementGroup measurementGroup = pool.getMeasurementGroupByDeviceName(deviceName);
			
			if(measurementGroup != null)
				return measurementGroup;
		}
		return null;
	}		
	
	public DevicePoolTreeNode[] getPoolNodes()
	{
		SardanaTreeNode machine = getMachineNode();
		
		if(machine == null)
			return new DevicePoolTreeNode[0];
		
		ArrayList<DevicePoolTreeNode> nodes = new ArrayList<DevicePoolTreeNode>();
		
		Enumeration poolNodes = machine.children();
		
		while(poolNodes.hasMoreElements())
		{
			SardanaTreeNode node = (SardanaTreeNode) poolNodes.nextElement();
			if (node instanceof DevicePoolTreeNode)
				nodes.add((DevicePoolTreeNode)node);
		}
		DevicePoolTreeNode[] ret = new DevicePoolTreeNode[nodes.size()];
		return nodes.toArray(ret);
	}
	
	public DevicePoolTreeNode getPoolNode(String pool)
	{
		SardanaTreeNode machine = getMachineNode();
		
		if(machine == null)
			return null;
		
		Enumeration poolNodes = machine.children();
		
		while(poolNodes.hasMoreElements())
		{
			SardanaTreeNode node = (SardanaTreeNode) poolNodes.nextElement();
			
			if(!(node instanceof DevicePoolTreeNode))
				continue;
			
			DevicePoolTreeNode poolNode = (DevicePoolTreeNode)node;
			
			DevicePool poolObj = poolNode.getDevicePool();
			if(poolObj == null)
				continue;
			
			if(poolObj.getName().equalsIgnoreCase(pool) || poolObj.getDeviceName().equalsIgnoreCase(pool))
			{
				return poolNode;
			}
		}
		return null;
	}
	
	protected void refreshPool(String poolDeviceName)
	{
		DevicePoolTreeNode poolNode = getPoolNode(poolDeviceName);
		
		if(poolNode == null)
		{
			log.warning("unable to find " + poolDeviceName + " to refresh it");
			return;
		}
		
		DevicePool devicePool = poolNode.getDevicePool();
		
		//devicePool.removeDevStateScalarListener(poolListener);
		devicePool.removeStatusListener(poolListener);

		devicePool.removeControllerListListener(controllerListListener);
		devicePool.removeCommunicationChannelListListener(comChannelListListener);
		devicePool.removeMotorListListener(motorListListener);
		devicePool.removeMotorGroupListListener(motorGroupListListener);
		devicePool.removePseudoMotorListListener(pseudoMotorListListener);
		devicePool.removeExperimentChannelListListener(expChannelListListener);
		devicePool.removeMeasurementGroupListListener(measurementGroupListListener);

		for(SardanaDevice dev : devicePool.getDeviceElements())
			dev.removeDevStateScalarListener(stateListener);
		
		MachineTreeNode machine = (MachineTreeNode) poolNode.getParent();
		
		poolNode.removeAllChildren();
		poolNode.removeFromParent();
		
		createDevicePoolTree(devicePool, machine);
		
		sardanaModel.nodeChanged(machine);
		sardanaModel.nodeStructureChanged(machine);
	}
	
	/*
	protected void shutdownPool(String poolDeviceName)
	{
		DevicePoolTreeNode poolNode = getPoolNode(poolDeviceName);
		
		if(poolNode == null)
		{
			log.warning("unable to find " + poolDeviceName + " to refresh it");
			return;
		}
		
		DevicePool devicePool = poolNode.getDevicePool();
		
		devicePool.removeDevStateScalarListener(poolListener);
		devicePool.removeStatusListener(poolListener);

		devicePool.removeControllerListListener(controllerListListener);
		devicePool.removeMotorListListener(motorListListener);
		devicePool.removeMotorGroupListListener(motorGroupListListener);
		devicePool.removePseudoMotorListListener(pseudoMotorListListener);
		devicePool.removeExperimentChannelListListener(expChannelListListener);
		devicePool.removeMeasurementGroupListListener(measurementGroupListListener);

		for(SardanaDevice dev : devicePool.getDeviceElements())
			dev.removeDevStateScalarListener(stateListener);
		
		poolNode.removeAllChildren();		
	}
	*/
	
	public MacroServerTreeNode[] getMacroServerNodes()
	{
		SardanaTreeNode machine = getMachineNode();
		
		if(machine == null)
			return new MacroServerTreeNode[0];
		
		ArrayList<MacroServerTreeNode> nodes = new ArrayList<MacroServerTreeNode>();
		
		Enumeration machineNodes = machine.children();
		
		while(machineNodes.hasMoreElements())
		{
			SardanaTreeNode node = (SardanaTreeNode) machineNodes.nextElement();
			if (node instanceof MacroServerTreeNode)
				nodes.add((MacroServerTreeNode)node);
		}
		MacroServerTreeNode[] ret = new MacroServerTreeNode[nodes.size()];
		return nodes.toArray(ret);
	}
	
	public MacroServerTreeNode getMacroServerNode(String msName)
	{
		SardanaTreeNode machine = getMachineNode();
		
		if(machine == null)
			return null;
		
		Enumeration machineNodes = machine.children();
		
		while(machineNodes.hasMoreElements())
		{
			SardanaTreeNode node = (SardanaTreeNode) machineNodes.nextElement();
			
			if(!(node instanceof MacroServerTreeNode))
				continue;
			
			MacroServerTreeNode msNode = (MacroServerTreeNode)node;
			
			MacroServer msObj = msNode.getMacroServer();
			if(msObj == null)
				continue;
			
			if(msObj.getName().equalsIgnoreCase(msName) || msObj.getDeviceName().equalsIgnoreCase(msName))
			{
				return msNode;
			}
		}
		return null;
	}	

	
	public void refreshDataModel()
	{
		// First clean all data.
		DevicePoolTreeNode[] poolNodes = getPoolNodes();
		
		for (DevicePoolTreeNode poolNode : poolNodes)
		{
			DevicePool devicePool = poolNode.getDevicePool();
			
			devicePool.removeDevStateScalarListener(poolListener);
			devicePool.removeStatusListener(poolListener);

			devicePool.removeControllerListListener(controllerListListener);
			devicePool.removeCommunicationChannelListListener(comChannelListListener);
			devicePool.removeMotorListListener(motorListListener);
			devicePool.removeMotorGroupListListener(motorGroupListListener);
			devicePool.removePseudoMotorListListener(pseudoMotorListListener);
			devicePool.removeExperimentChannelListListener(expChannelListListener);
			devicePool.removeMeasurementGroupListListener(measurementGroupListListener);

			for(SardanaDevice dev : devicePool.getDeviceElements())
				dev.removeDevStateScalarListener(stateListener);
		}
		
		MacroServerTreeNode[] msNodes = getMacroServerNodes();
	
		for (MacroServerTreeNode msNode : msNodes)
		{
			MacroServer macroServer = msNode.getMacroServer();
			
			macroServer.removeDevStateScalarListener(poolListener);
			macroServer.removeStatusListener(poolListener);
		}		
		
		SardanaRootTreeNode rootNode = getRootNode();
		rootNode.removeAllChildren();
		
		// Now build the tree structure
		
		String hostName = currentPreferences.getTangoHostName();
		String hostPort = currentPreferences.getTangoHostPort();
		
		Machine machine = new Machine(hostName, hostPort);
		MachineTreeNode hostNode = new MachineTreeNode(machine);
		rootNode.add(hostNode);
		
		ArrayList<String> existingDevicePools = utils.getPoolDevices(machine);
		ArrayList<String> availableDevicePools = utils.getAvailableDevicePools(machine);
		
		String filter = currentPreferences.getPoolFilter();
		
		Pattern pattern = null;
		boolean doFilter = false;
		if(filter != null && filter.length() > 0)
		{
			doFilter = true;
			pattern = Pattern.compile(filter, Pattern.CASE_INSENSITIVE);
		}
		
		for(String deviceName : existingDevicePools)
		{
			if(doFilter)
				if(!pattern.matcher(deviceName).matches())
					continue;
			
//			//TODO Avoiding the Unit test Device Pool. In future this condition should be removed
			if(deviceName.equals("et/pool/test"))
				continue;
			
			boolean available = availableDevicePools.contains(deviceName);
			
			//First we create the node and attach it to the tree. Only after we attach a 
			//real device to the Device Pool. This is done to prevent receiving attribute
			//events before a real node in the tree exists.
			
			DevicePool devicePool = utils.getNewDevicePool(machine, deviceName, available);
			
			createDevicePoolTree(devicePool, hostNode);
		}
		
		ArrayList<String> existingMacroServers = utils.getMacroServerDevices(machine);
		ArrayList<String> availableMacroServers = utils.getAvailableMacroServers(machine);
		
		for(String deviceName : existingMacroServers)
		{
			boolean available = availableMacroServers.contains(deviceName);
			
			//First we create the node and attach it to the tree. Only after we attach a 
			//real device to the Device Pool. This is done to prevent receiving attribute
			//events before a real node in the tree exists.
			
			MacroServer macroServer = utils.getNewMacroServer(machine, deviceName, available);
			
			createMacroServerTree(macroServer, hostNode);
		}		
		sardanaModel.nodeChanged(rootNode);
		sardanaModel.nodeStructureChanged(rootNode);
	}
	
	/**
	 * 
	 * @param devicePool
	 * @param hostNode
	 * @return
	 */
	private DevicePoolTreeNode createDevicePoolTree(DevicePool devicePool, MachineTreeNode hostNode)
	{
		DevicePoolTreeNode poolNode = new DevicePoolTreeNode(devicePool);
		
		hostNode.add( poolNode );
		
		if(devicePool.isAvailable())
		{
			devicePool.addDevStateScalarListener(poolListener);
			devicePool.addStatusListener(poolListener);

			devicePool.addControllerListListener(controllerListListener);
			devicePool.addCommunicationChannelListListener(comChannelListListener);
			devicePool.addMotorListListener(motorListListener);
			devicePool.addMotorGroupListListener(motorGroupListListener);
			devicePool.addPseudoMotorListListener(pseudoMotorListListener);
			devicePool.addExperimentChannelListListener(expChannelListListener);
			devicePool.addMeasurementGroupListListener(measurementGroupListListener);
			
			createControllerTree(poolNode);
			createComChannelsTree(poolNode);
			createMotorsTree(poolNode);
			createMotorGroupsTree(poolNode);
			createExpChannelsTree(poolNode);
			createMeasurementGroupsTree(poolNode);
			//createPseudoMotorsTree(poolNode);

//			for(SardanaDevice dev : devicePool.getDeviceElements())
//				dev.addDevStateScalarListener(dev);
		
		}
		return poolNode;
	}
	
	private MacroServerTreeNode createMacroServerTree(MacroServer macroServer, MachineTreeNode hostNode)
	{
		MacroServerTreeNode macroServerNode = new MacroServerTreeNode(macroServer);
		
		hostNode.add( macroServerNode );
		
		if(macroServer.isAvailable())
		{
			macroServer.addDevStateScalarListener(macroServerListener);
			macroServer.addStatusListener(macroServerListener);
		}
		
		createDoorsTree(macroServerNode);
		
		return macroServerNode;
	}
	
	private void createDoorsTree(MacroServerTreeNode macroServerNode) 
	{
		MacroServer macroServer = macroServerNode.getMacroServer();

		for(Door d : macroServer.getDoors())
		{
			DoorTreeNode n = new DoorTreeNode(d);
			macroServerNode.add(n);
			d.addDevStateScalarListener(stateListener);
		}
	}	
	
	private void createSardanaDataModel()
	{
		SardanaRootTreeNode rootNode = new SardanaRootTreeNode("Sardana");
		
		sardanaModel = new SardanaDataModel(rootNode);
		
		stateListener = new StateListener();
		poolListener = new DevicePoolListener();
		macroServerListener = new MacroServerListener();
		controllerListListener = new ControllerListListener();
		comChannelListListener = new ComChannelListListener();
		motorListListener = new MotorListListener();
		motorGroupListListener = new MotorGroupListListener();
		pseudoMotorListListener = new PseudoMotorListListener();
		expChannelListListener = new ExpChannelListListener();
		measurementGroupListListener = new MeasurementGroupListListener();
		
		refreshDataModel();
	}
	
	private void createControllerTree(DevicePoolTreeNode poolNode) 
	{
		DevicePool pool = poolNode.getDevicePool();
		
		if(pool.isAvailable())
		{
			ControllersTreeNode ctrlsNode = new ControllersTreeNode("Controllers", pool);
			poolNode.setControllersNode(ctrlsNode);
		}
	}

	private void createComChannelsTree(DevicePoolTreeNode poolNode) 
	{
		DevicePool pool = poolNode.getDevicePool();

		if(pool.isAvailable())
		{
			ComChannelsTreeNode comChannelsNode = new ComChannelsTreeNode("Communication Channels", pool);
			poolNode.setComChannelsNode(comChannelsNode);
		}
		
		for(CommunicationChannel comChannel : pool.getCommunicationChannels())
			comChannel.addDevStateScalarListener(stateListener);
	}	
	
	private void createMotorsTree(DevicePoolTreeNode poolNode) 
	{
		DevicePool pool = poolNode.getDevicePool();

		if(pool.isAvailable())
		{
			MotorsTreeNode motorsNode = new MotorsTreeNode("Motors", pool);
			poolNode.setMotorsNode(motorsNode);
		}
		
		for(Motor m : pool.getMotors())
			m.addDevStateScalarListener(stateListener);
		for(PseudoMotor pm : pool.getPseudoMotors())
			pm.addDevStateScalarListener(stateListener);

	}

	private void createMotorGroupsTree(DevicePoolTreeNode poolNode) 
	{
		DevicePool pool = poolNode.getDevicePool();

		if(pool.isAvailable())
		{
			MotorGroupsTreeNode motorGroupsNode = new MotorGroupsTreeNode("Motor Groups", pool);
			poolNode.setMotorGroupsNode(motorGroupsNode);
		}

		for(MotorGroup mg : pool.getMotorGroups())
			mg.addDevStateScalarListener(stateListener);
}
	
	private void createExpChannelsTree(DevicePoolTreeNode poolNode) 
	{
		DevicePool pool = poolNode.getDevicePool();

		if(pool.isAvailable())
		{
			ExpChannelsTreeNode channelsNode = new ExpChannelsTreeNode("Experiment Channels", pool);
			poolNode.setExpChannelsNode(channelsNode);
		}

		for(ExperimentChannel ch : pool.getExperimentChannels())
			ch.addDevStateScalarListener(stateListener);

	}

	private void createMeasurementGroupsTree(DevicePoolTreeNode poolNode) 
	{
		DevicePool pool = poolNode.getDevicePool();

		if(pool.isAvailable())
		{
			MeasurementGroupsTreeNode measurementGroupsNode = new MeasurementGroupsTreeNode("Measurement Groups", pool);
			poolNode.setMeasurementGroupsNode(measurementGroupsNode);
		}
		
		for(MeasurementGroup mg : pool.getMeasurementGroups())
			mg.addDevStateScalarListener(stateListener);
	}
	
	protected class StateListener implements IDevStateScalarListener
	{
		public void devStateScalarChange(DevStateScalarEvent evt) 
		{
			Device device = ((DevStateScalar)evt.getSource()).getDevice();
			
			SardanaTreeNode node = getSardanaNodeByDeviceName(device.getName());
			
			if(node == null)
			{
				log.warning("Unable to find sardana node for device: " + device.getName() );
			}
			else
			{
				sardanaModel.nodeChanged(node);
			}
		}

		public void stateChange(AttributeStateEvent evt) {}
		public void errorChange(ErrorEvent evt) {}
	}
	
	
	class DevicePoolListener implements IDevStateScalarListener, IStringScalarListener
	{
		public void devStateScalarChange(DevStateScalarEvent evt)
		{
			// If the pool is preparing to shutdown don't do anything.
			// Just let it shutdown quietly
			if(evt.getValue().equalsIgnoreCase("MOVING"))
				return;
				
			Device device = ((DevStateScalar)evt.getSource()).getDevice();

			final DevicePoolTreeNode node = getPoolNode(device.getName());
			
			if(node == null)
				return;
			
			sardanaModel.nodeChanged(node);
			
			if(evt.getValue().equalsIgnoreCase("UNKNOWN") ||
			   evt.getValue().equalsIgnoreCase("ON"))
			{
				refreshPool(device.get_name());
			}
			
			if(!evt.getValue().equalsIgnoreCase("UNKNOWN"))
			{
				// If a state event has been received then update the Status
				node.getDevicePool().getStatusAttributeModel().refresh();
			}
		}
		
		public void stateChange(AttributeStateEvent evt)
		{
		}

		public void stringScalarChange(StringScalarEvent evt)
		{
			Device d = ((StringScalar)evt.getSource()).getDevice();
			
			DevicePoolTreeNode node = getPoolNode(d.getName());
			
			if(node == null)
				return;
			
			DevicePool devicePool = node.getDevicePool();
			
			String newStatus = evt.getValue();
			
			boolean ctrlRequiresAttention = newStatus.contains(DevicePoolUtils.POOL_STATUS_CTRL_ERROR);
			
			if(ctrlRequiresAttention) 
			{
				String [] errorList = DevicePoolUtils.POOL_STATUS_CTRL_ERROR_SPLIT_PATTERN.split(newStatus);
				
				//First clear the state of all controllers
				for(Controller ctrl : devicePool.getControllers())
					ctrl.setState(CtrlState.Ok);
				
				for(String errorMsg : errorList)
				{
					Matcher matcher = DevicePoolUtils.POOL_STATUS_CTRL_ERROR_PATTERN.matcher(errorMsg);
					
					if(matcher.matches())
					{
						//String className = matcher.group( DevicePoolUtils.POOL_STATUS_CTRL_ERROR_CTRL_CLASS_INDEX);
						//String libName = matcher.group( DevicePoolUtils.POOL_STATUS_CTRL_ERROR_CTRL_LIB_INDEX);
						String ctrlInstance = matcher.group( DevicePoolUtils.POOL_STATUS_CTRL_ERROR_CTRL_INSTANCE_INDEX);
					
						Controller ctrl = devicePool.getController(ctrlInstance);
						
						if(ctrl == null)
							continue;
						
						ctrl.setState(CtrlState.Error);
					}
				}
			}			
		}

		public void errorChange(ErrorEvent evt)
		{
			// If you have continuous calls to this method with evt.type = Event_Timeout
			// it could mean that the client app is running on a linux (probably a SuSE distribution) 
			// which has in /etc/hosts the line: 127.0.0.x   [local pc name]
			// If this is the case you need to comment this line (requires to login as root!!!)
			// The problem is omniORB translates your client PC name into 127.0.0.x and registers this
			// IP in the server to get the heartbeat. This is of course an invalid IP address so 
			// the heartbeat never arrives in to the client.
			//
			// Another cause for this problem could be the firewall settings of the client.
			// 
			// I (Tiago Coutinho) got this information from Emmanuel Taurel.
			
			System.out.println("ERROR EVENT: " + evt.toString());
			
			if(evt.getSource() == null)
				return;
			
			if(! (evt.getSource() instanceof DevStateScalar))
				return;
			
			Device d = ((DevStateScalar)evt.getSource()).getDevice();
			
			DevicePoolTreeNode node = SardanaManager.getInstance().getPoolNode(d.getName());

			if(!node.getState().equals("UNKNOWN"))
			{
				log.warning(node.getDevicePool().getName() + " reported ERROR EVENT: " + evt.toString());
				sardanaModel.nodeChanged(node);
			}
		}
	}
	
	class MacroServerListener implements IDevStateScalarListener, IStringScalarListener
	{
		public void devStateScalarChange(DevStateScalarEvent evt)
		{
			Device device = ((DevStateScalar)evt.getSource()).getDevice();

			final MacroServerTreeNode node = getMacroServerNode(device.getName());
			
			if(node == null)
				return;
			
			sardanaModel.nodeChanged(node);
			
			if(evt.getValue().equalsIgnoreCase("UNKNOWN") ||
			   evt.getValue().equalsIgnoreCase("ON"))
			{
				refreshPool(device.get_name());
			}
			
			if(!evt.getValue().equalsIgnoreCase("UNKNOWN"))
			{
				// If a state event has been received then update the Status
				node.getMacroServer().getStatusAttributeModel().refresh();
			}
		}
		
		public void stateChange(AttributeStateEvent evt)
		{
		}

		public void stringScalarChange(StringScalarEvent evt)
		{	
		}

		public void errorChange(ErrorEvent evt)
		{
			System.out.println("ERROR EVENT: " + evt.toString());
			
			if(evt.getSource() == null)
				return;
			
			if(! (evt.getSource() instanceof DevStateScalar))
				return;
			
			Device d = ((DevStateScalar)evt.getSource()).getDevice();
			
			MacroServerTreeNode node = SardanaManager.getInstance().getMacroServerNode(d.getName());

			if(!node.getState().equals("UNKNOWN"))
			{
				log.warning(node.getMacroServer().getName() + " reported ERROR EVENT: " + evt.toString());
				sardanaModel.nodeChanged(node);
			}
		}
	}
	
	protected abstract class ConcurrentListListener implements IStringSpectrumListener
	{
		protected abstract ElementListTreeNode getNode(DevicePoolTreeNode poolNode);
		
		public void stringSpectrumChange(StringSpectrumEvent e)
		{
			final StringSpectrumEvent evt = e;
			Runnable doWork = new Runnable() 
			{
				public void run()
				{
					String poolName = "";
					if(evt instanceof PoolInternalStringSpectrumEvent)
					{
						poolName = ((PoolInternalStringSpectrumEvent)evt).getPool().getDeviceName();
					}
					else
					{
						StringSpectrum model = (StringSpectrum)evt.getSource();
						poolName = model.getDevice().getName();
					}
					
					DevicePoolTreeNode poolNode = getPoolNode(poolName);
					
					if(poolNode == null)
					{
						log.warning("Could not find Pool node for :" + poolName +". Tree nodes will not be updated");
						return;
					}
						
					ElementListTreeNode node = getNode(poolNode);
					
					node.updateChilds();
					
					sardanaModel.nodeChanged(node);
					sardanaModel.nodeStructureChanged(node);
				}
			};
			SwingUtilities.invokeLater(doWork);
		}

		public void stateChange(AttributeStateEvent e) { /* Quality of the attribute changed*/ }
		public void errorChange(ErrorEvent evt)	{}		
	}
	
	protected class ControllerListListener extends ConcurrentListListener
	{
		protected ElementListTreeNode getNode(DevicePoolTreeNode poolNode)
		{
			return poolNode.getControllersNode();
		}
	}
	
	protected class ComChannelListListener extends ConcurrentListListener
	{
		protected ElementListTreeNode getNode(DevicePoolTreeNode poolNode)
		{
			return poolNode.getComChannelsNode();
		}
	}
	
	protected class MotorListListener extends ConcurrentListListener
	{
		protected ElementListTreeNode getNode(DevicePoolTreeNode poolNode)
		{
			return poolNode.getMotorsNode();
		}
	}
	
	protected class MotorGroupListListener extends ConcurrentListListener
	{
		protected ElementListTreeNode getNode(DevicePoolTreeNode poolNode)
		{
			return poolNode.getMotorGroupsNode();
		}
	}
	
	protected class PseudoMotorListListener extends ConcurrentListListener
	{
		protected ElementListTreeNode getNode(DevicePoolTreeNode poolNode)
		{
			return poolNode.getMotorsNode();
		}
	}

	protected class ExpChannelListListener extends ConcurrentListListener
	{
		protected ElementListTreeNode getNode(DevicePoolTreeNode poolNode)
		{
			return poolNode.getExpChannelsNode();
		}
	}
	
	protected class MeasurementGroupListListener extends ConcurrentListListener
	{
		protected ElementListTreeNode getNode(DevicePoolTreeNode poolNode)
		{
			return poolNode.getMeasurementGroupsNode();
		}
	}

	public JMenuBar getMenuBar() 
	{
		if(menuBar != null) return menuBar;
		
		menuBar = new JMenuBar();
		
		JMenu fileMenu = new JMenu("File");
		
		JMenuItem loadConfig = new JMenuItem(new AbstractAction("Load configuration file")
		{
			public void actionPerformed(ActionEvent e) 
			{
				JFileChooser fileChooser = new JFileChooser();
				
				FileFilter filter = new FileFilter() 
				{
					@Override
					public boolean accept(File f) 
					{if(f.isDirectory()) return true;
						return f.getName().endsWith(".xsr");
					}

					@Override
					public String getDescription() 
					{
						return "Sardana XML configuration file (*.xsr)";
					}
				};
				
				fileChooser.addChoosableFileFilter(filter);
				fileChooser.setFileSelectionMode(JFileChooser.FILES_ONLY);
				fileChooser.setFileView(SwingResource.getPoolFileView());
				int retval = fileChooser.showOpenDialog(null);
				
				if(retval == JFileChooser.APPROVE_OPTION)
				{
					File f = fileChooser.getSelectedFile();
					if( f != null )
					{
						try 
						{
							Machine m = SardanaManager.getInstance().getMachineNode().getMachine();
							
							Vector<Machine> machines = new Vector<Machine>();
							machines.add(m);
							
							m = (Machine)JOptionPane.showInputDialog(null, 
									"Select machine to load configuration to", 
									"Select machine", 
									JOptionPane.PLAIN_MESSAGE,
									SwingResource.bigIconMap.get(IImageResource.IMG_MACHINE_RUN),
									machines.toArray(),
									m);
							
							if(m != null)
							{
								PoolConfigLoader loader = new PoolConfigLoader(f, m);
								SardanaConfigEditor editor = new SardanaConfigEditor(loader);
								editor = null;
							}
						}
						catch (XmlException e1) 
						{
							JOptionPane.showMessageDialog(null, e1.getError().toString(), e1.getMessage(), JOptionPane.ERROR_MESSAGE);
						} 
						catch (IOException e2) 
						{
							JOptionPane.showMessageDialog(null, e2.getMessage(), e2.getMessage(), JOptionPane.ERROR_MESSAGE);
						}
						}
					
				}
			}
		});
		loadConfig.setIcon(SwingResource.smallIconMap.get(IImageResource.IMG_FILE_OPEN));
		
		JMenuItem saveConfig = new JMenuItem(new AbstractAction("Save configuration file")
		{
			public void actionPerformed(ActionEvent e) 
			{
				JFileChooser fileChooser = new JFileChooser();
				
				FileFilter filter = new FileFilter() 
				{
					@Override
					public boolean accept(File f) 
					{
						if(f.isDirectory()) return true;
						return f.getName().endsWith(".xsr");
					}

					@Override
					public String getDescription() 
					{
						return "Sardana XML configuration file (*.xsr)";
					}
				};
				
				fileChooser.addChoosableFileFilter(filter);
				fileChooser.setFileSelectionMode(JFileChooser.FILES_ONLY);
				fileChooser.setFileView(SwingResource.getPoolFileView());
				int retval = fileChooser.showSaveDialog(null);
				
				if(retval == JFileChooser.APPROVE_OPTION)
				{
					File f = fileChooser.getSelectedFile();
					if( f != null )
					{
						List<DevicePool> pools = new ArrayList<DevicePool>();
						TreePath[] elems = leftPanel.getTree().getSelectionPaths();
						for(TreePath elem : elems)
						{
							Object lastInSelection = elem.getPathComponent(elem.getPathCount()-1);
							if(lastInSelection instanceof DevicePoolTreeNode)
							{
								DevicePoolTreeNode poolNode = (DevicePoolTreeNode)lastInSelection;
								DevicePool pool = poolNode.getDevicePool();
								if(!pools.contains(pool))
									pools.add(pool);
							}
						}
						try 
						{
							PoolConfigSaver saver = new PoolConfigSaver(f,pools);
							PoolsDocument doc = saver.build(loadedPreferences);
							
							SardanaConfigView previewer = new SardanaConfigView(doc);
							previewer.setTitle("Preview");
							
							saver.save(doc);
							
							JOptionPane.showMessageDialog(null,	
									"Configuration saved to " + f.getName(), 
									"Success!", JOptionPane.INFORMATION_MESSAGE);
						} 
						catch (IOException e1) 
						{
							JOptionPane.showMessageDialog(null,	
									e1.getMessage(), 
									"Error trying to save configuration!", JOptionPane.ERROR_MESSAGE);
						}
					}
				}
			}
		});
		saveConfig.setIcon(SwingResource.smallIconMap.get(IImageResource.IMG_FILE_SAVE));
		
		fileMenu.add(loadConfig);
		fileMenu.add(new JSeparator());
		fileMenu.add(saveConfig);
		
		menuBar.add(fileMenu);
		
		JMenu createMenu = new JMenu("Create");
		
		JMenuItem createPool = new JMenuItem(new AbstractAction("Create Device Pool")
		{
			public void actionPerformed(ActionEvent e) 
			{
				AddPoolDialog dialog = new AddPoolDialog(getMachineNode().getMachine());
				dialog.setLocationRelativeTo(null);
				dialog.setVisible(true);
			}
		});
		createPool.setIcon(SwingResource.smallIconMap.get(IImageResource.IMG_POOL));
		
		createMenu.add(createPool);

		JMenuItem createMS = new JMenuItem(new AbstractAction("Create Macro Server")
		{
			public void actionPerformed(ActionEvent e) 
			{
				AddMacroServerDialog dialog = new AddMacroServerDialog(getMachineNode().getMachine());
				dialog.setLocationRelativeTo(null);
				dialog.setVisible(true);
			}
		});
		createMS.setIcon(SwingResource.smallIconMap.get(IImageResource.IMG_MACROSERVER));
		
		createMenu.add(createMS);
		
		menuBar.add(createMenu);
		
		return menuBar;
	}
	
	
}
