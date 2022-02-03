package observer;

import java.util.logging.Level;
import java.util.logging.Logger;

public abstract class Observer{
    private final Logger logger = Logger.getLogger(Observer.class.getName());

    protected Observer(){
    }

    public void write(){
        logger.log(Level.INFO, "Notified");
    }

    public abstract int update();
}