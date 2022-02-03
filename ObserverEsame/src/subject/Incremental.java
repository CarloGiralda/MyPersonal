package subject;
import observer.*;

public class Incremental extends Sub {
    int state;
    int prev;

    public Incremental(){
        super();
        state=0;
        prev=0;
    }

    public Incremental(Observer ob){
        super(ob);
        state=0;
        prev=0;
    }

    public int getStatus(){
        return state;
    }

    public boolean isUpdated(){
        if(prev==state){
            prev=state;
            return false;
        }else{
            prev=state;
            return true;
        }
    }

    public void increment(){
        state+=10;
        this.notifyObservers();
    }

    public static void main(String[] args){
        Incremental incr=new Incremental();
        Up obs=new Up(incr);
        incr.attach(obs);
        incr.increment();
        incr.increment();
        incr.detach(obs);
    }
}
