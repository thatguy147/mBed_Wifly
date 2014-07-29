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
  
  int buffer_pos = 0;
  char buffer[50];
  
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
       //serial_spi.writeString("\rSendingSomething");
       //pc.printf("SendingSomethingOnSerial");
   }
   
   std::string intToString(int i)
    {
        std::stringstream ss;
        std::string s;
        ss << std::hex << i;
        s = (i == 0) ? "00" : ss.str();

        return s;
    }
    
    CANMessage makeCANMessage()
    {
        CANMessage can_m;
        int space = 0;
        for(int i = 4; i<buffer_pos; i++)
        {
            char c = (char)buffer[i];
            if(c == ' ')
            {
                space = i;
                break;
            }
        }    
        string idStr(buffer, 4,space-4);
        int id;
        istringstream ( idStr ) >> id;
        
        char Cdata[8];
        for(int i = 1; i < 9; i++)
        {
            pc.printf("\r\n %d", space);
            string DBStr(buffer,(space+1),2);
            pc.printf("%s\n",DBStr);
            int DB;
            istringstream (DBStr) >> DB;
            //pc.printf("\r\n%d",DB);
            space += 3;
            
            can_m.data[i-1] = DB;
        }
        can_m.id = id;
        //can_m.data = Cdata;
        can1.write(can_m);
        //int id = std
        return can_m;
    }
       
    void handleInput()
    {
        pc.printf("\r\nbuffersize %d\r\n", buffer_pos);
        pc.printf(buffer);
        
        CANMessage can_m = makeCANMessage();
        
        buffer[0] = '\0';
        
        buffer_pos = 0;
        
    }
    
        

    void terminal()
    {
        bool record = false;
        
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
                //pc.printf("%d", serial_spi.getc());
                //pc.printf("%c", serial_spi.getc());
                
                char c = (char)serial_spi.getc();
                pc.printf("%c",c);
                if(c == '<') //<
                {
                    pc.printf("got start");
                    record = true;
                } 
                if(c == '>')// >
                {
                    pc.printf("got end");
                    buffer[buffer_pos] = c;
                    buffer_pos++;
                    handleInput();
                    record = false;
                }
                
                if(record == true)
                {
                    buffer[buffer_pos] = c;
                    buffer_pos++;
                }           
                //pc.printf("%c", serial_spi.getc());
                
            }
            /*if(!serial_spi.readable() && buffer_pos > 28)
            {
                pc.printf("\n");
                pc.printf(buffer);
                handleInput();
            }*/
        
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
                //pc.printf("RX %d",can_MsgRx.data[0]);
                
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
        pc.printf("%d", '<');
        pc.printf("%d", '>');
        pc.printf("\nHello World!\n");
        initWifly();
        //startReading();
        terminal();
        //while(1)
        {
        //    sendSomething();
        }
    }
