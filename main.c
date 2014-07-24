//SPI spi_device(p11, p12, p13);
//DigitalOut spi_cs(p8);
 
  #include "mbed.h"
  #include "SC16IS750.h"
  #include <string> 
  
  SPI spi(p11, p12, p13); //MOSI, MISO, SCK
  SC16IS750_SPI serial_spi(&spi, p8);
  
  string IP = "192.168.1.11";
  string port = "5005";
  string open = "open";
  
  Serial pc(USBTX,USBRX);
  
  void initWifly()
  {      
        if(serial_spi.connected())
        {
            serial_spi.writeString("\r");
            serial_spi.writeString("$$$");
            wait(0.2); 
            serial_spi.writeString("\r");
            serial_spi.writeString("get everything\r\n");
            wait(0.2);
            const string openConnection = (open + " " + IP + " "+ port +"\r");
            char * writable = new char[openConnection.size() + 1];
            serial_spi.writeString(writable); 
        }
   } 
   
   void sendSomething()
   {
       serial_spi.writeString("\r\rSendingSomething");
       pc.printf("SendingSomethingOnSerial");
   }    

    void terminal()
    {
        while(1)
        {
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

    int main()
    {
        
        pc.printf("\nHello World!\n");
        initWifly();
        terminal();
        //while(1)
        {
        //    sendSomething();
        }
    }
