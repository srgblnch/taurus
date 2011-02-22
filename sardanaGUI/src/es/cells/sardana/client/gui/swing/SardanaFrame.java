package es.cells.sardana.client.gui.swing;

import javax.swing.JFrame;
import javax.swing.UIManager;

import org.jvnet.substance.SubstanceLookAndFeel;
import org.jvnet.substance.skin.SubstanceBusinessLookAndFeel;
import org.jvnet.substance.skin.SubstanceOfficeSilver2007LookAndFeel;
import org.jvnet.substance.theme.SubstanceDesertSandTheme;
import org.jvnet.substance.theme.SubstanceTheme;

import es.cells.sardana.client.framework.gui.IImageResource;
import fr.esrf.tangoatk.widget.util.Splash;

@SuppressWarnings("serial")
public class SardanaFrame extends JFrame 
{
	protected SardanaMainPanel mainPanel;

	
	public SardanaFrame(Splash splash)
	{
		super("Sardana");
		initComponents(splash);
	}
	
	protected void initComponents(Splash splash)
	{
		setDefaultCloseOperation(javax.swing.WindowConstants.EXIT_ON_CLOSE);
		
		setIconImage(SwingResource.smallIconMap.get(IImageResource.IMG_SARDANA).getImage());
		
		try
		{
			
			//TODO: Substance look and feel
			
			SubstanceLookAndFeel laf = new SubstanceLookAndFeel();
			
			UIManager.setLookAndFeel(laf);
			
			//Change Skin
			
			//Change Theme
			/*
			SubstanceSteelBlueTheme theme = new SubstanceSteelBlueTheme();
			SubstanceLookAndFeel.setCurrentTheme(theme);
			
			//Change Watermark
			SubstanceNullWatermark watermark = new SubstanceNullWatermark();
			SubstanceLookAndFeel.setCurrentWatermark(watermark);
			
			PlasticLookAndFeel.setPlasticTheme(new ExperienceBlue());
			*/
			try 
			{
				SubstanceTheme theme = new SubstanceDesertSandTheme();
				UIManager.setLookAndFeel(new SubstanceBusinessLookAndFeel());
				SubstanceOfficeSilver2007LookAndFeel.setCurrentTheme(theme);
			} 
			catch (Exception e) {}
			
			mainPanel = new SardanaMainPanel(splash);
			getContentPane().add(mainPanel);
			setJMenuBar(mainPanel.getMenuBar());
			this.pack();
			this.setVisible(true);
			
		}
		catch(Exception e)
		{
			e.printStackTrace();
		}
	}
}
