package observer;

import subject.Incremental;

public class Up extends Observer {
    int number;
    Incremental sub;

    public Up(Incremental subject){
        super();
        number=0;
        this.sub=subject;
    }

    @Override
    public int update() {
        if (this.sub == null){
            return 0;
        }else{
            number = this.sub.getStatus();
            this.write();
            if(number>15){
                return 1;
            }
            return 0;
        }
    }
}