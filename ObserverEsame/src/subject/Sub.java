package subject;

import observer.Observer;

import java.util.ArrayList;
import java.util.List;

public abstract class Sub {
    private List<Observer> observers;

    protected Sub(){
        observers=new ArrayList<>();
    }

    protected Sub(Observer obs) {
        observers=new ArrayList<>();
        if (obs != null)
            this.observers.add(obs);
    }

    public void attach(Observer obs) {
        this.observers.add(obs);
    }

    public void detach(Observer obs) {
        this.observers.remove(obs);
    }

    protected void notifyObservers() {
        if (this.isUpdated()){
            for(int i=0; i< observers.size(); i++){
                observers.get(i).update();
            }
        }
    }

    protected abstract boolean isUpdated();

    protected abstract void increment();
}