package es.cells.sardana.client.framework.pool;

public enum ControllerType 
{
	InvalidControllerType,
	Motor,
	CounterTimer,
	ZeroDExpChannel,
	Communication,
	PseudoMotor,
	PseudoCounter;

	@Override
	public String toString()
	{
		return super.toString();
	}
	
	
}
