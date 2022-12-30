TCCR1A &= ~(1<<WgM10)
TCCR1A &= ~(1<<WgM11)
TCCR1A &= ~(1<<WgM12)
TCCR1A &= ~(1<<WgM13)

TCCR1B &= ~1<<CS10
TCCR1B &= ~1<<CS11    //0
TCCR1B |= 1<<CS12     //1
//interupts pjrc.com
//enable global interupts
//pg 413
SEI();

//Overflow interupt pg 136
//TOIEn    // n= timer number
TIMSK1 |= 1<<TOIE1;
//unsigned use less memory
volatile unsigned int counter;
//setup interupt pg 61
ISR(TIMER1_OVF_vect){
  counter++
}


//CTC clear on compare
