//SPI spi_device(p11, p12, p13);
//DigitalOut spi_cs(p8);
 
  #include "mbed.h"
  #include "sstream"
  #include "SC16IS750.h"
  #include <string> 
  CAN can1(p9, p10);      // CAN1 uses pins 9 and 10 (tx, rx)
  SPI spi(p11, p12, p13); //MOSI, MISO, SCK
  SC16IS750_SPI serial_spi(&spi, p8);
  CANMessage can_MsgRx;
  
  
  string IP = "192.168.1.11";
  string port = "6001";
  string open = "open";
  
  Serial pc(USBTX,USBRX);
  
  void initWifly()
  {     
    can1.frequency(500000); 
        if(serial_spi.connected())
        {
            serial_spi.writeString("\r");
            serial_spi.writeString("$$$");
            wait(0.2); 
            serial_spi.writeString("\r\n");
            //serial_spi.writeString("get everything\r\n");
            wait(0.2);
            string openConnection = (open + " " + IP + " "+ port +"\r\n");
            //pc.printf(";
            //char * writable = new char[openConnection.size() + 1];
            
            std::string str = (open + " " + IP + " "+ port +"\r\n");
            char * writable = new char[str.size() + 1];
            std::copy(str.begin(), str.end(), writable);
            writable[str.size()] = '\0'; // don't forget the terminating 0
            serial_spi.writeString(writable);
            serial_spi.writeString("exit\r\n");
// don't forget to free the string after finished using it
            delete[] writable;
            
            //pc.printf(writable); 
            //serial_spi.writeString(writable); 
        }
   } 
   
   void sendSomething()
   {
       serial_spi.writeString("\rSendingSomething");
       pc.printf("SendingSomethingOnSerial");
   }
   
   std::string intToString(int i)
    {
        std::stringstream ss;
        std::string s;
        ss << std::hex << i;
        s = (i == 0) ? "00" : ss.str();

        return s;
    }    

    void terminal()
    {
        
        
        while(1)
        {
            if (can1.read(can_MsgRx))
            {
                pc.printf("RX");
                
                std::string _id = intToString(can_MsgRx.id);
                std::string _DB0 = intToString(can_MsgRx.data[0]);
                std::string _DB1 = intToString(can_MsgRx.data[1]);
                std::string _DB2 = intToString(can_MsgRx.data[2]);
                std::string _DB3 = intToString(can_MsgRx.data[3]);
                std::string _DB4 = intToString(can_MsgRx.data[4]);
                std::string _DB5 = intToString(can_MsgRx.data[5]);
                std::string _DB6 = intToString(can_MsgRx.data[6]);
                std::string _DB7 = intToString(can_MsgRx.data[7]);
                
                string str = "<" + _id + " " + _DB0 + " " + _DB1 + " " + _DB2  + " " + _DB3 + " " + _DB4 + " " + _DB5 + " " + _DB6 + " " + _DB7 + ">\r";
                std::string id = str;
                char * writable = new char[str.size() + 1];
                std::copy(str.begin(), str.end(), writable);
                writable[str.size()] = '\0'; // don't forget the terminating 0
                pc.printf("\n");
                pc.printf(writable);
                serial_spi.writeString(writable);
                delete[] writable;
            }
            
            while(serial_spi.readable())
            {
                pc.printf("%c", serial_spi.getc());
            }
        
            if(pc.readable())
            {
                if(pc.getc() == ']')
                {
                    sendSomething();
                }
                else{
                    serial_spi.putc(pc.getc());
                }   
            }
              
        }
    }
    
    void startReading()
    {
        can1.frequency(500000);
        while(1){
            if (can1.read(can_MsgRx))
            {
            // When sending a message to the PC, send a couple of sync bytes so that when the PC looks, it finds the start of a message
            // putc will write a character/byte to the pc, getc reads a character/byte from the pc
                pc.printf("RX %d",can_MsgRx.data[0]);
                
                int a = can_MsgRx.data[0];
                stringstream ss;
                ss << a;
                string str = "\rID: " + ss.str()+"\r";
                std::string id = str;
                //std::string b0 = std::to_string(can_MsgRx.data[0]);
                //std::string str = ("ID: " + id + " " + b0);
                char * writable = new char[str.size() + 1];
                std::copy(str.begin(), str.end(), writable);
                writable[str.size()] = '\0'; // don't forget the terminating 0
                serial_spi.writeString(writable);
                delete[] writable;
                
            }
    }    
}
    int main()
    {
        
        pc.printf("\nHello World!\n");
        initWifly();
        //startReading();
        terminal();
        //while(1)
        {
        //    sendSomething();
        }
    }
