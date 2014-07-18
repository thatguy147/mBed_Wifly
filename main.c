//SPI spi_device(p11, p12, p13);
//DigitalOut spi_cs(p8);
 
  #include "mbed.h"
  #include "SC16IS750.h"
 
  SPI spi(p11, p12, p13); //MOSI, MISO, SCK
  SC16IS750_SPI serial_spi(&spi, p8);
  
  Serial pc(USBTX,USBRX);
 
  void initWifly()
  {
        if(serial_spi.connected())
        {
            serial_spi.writeString("$$$\r");
            wait(0.5);
            serial_spi.writeString("get everything\r\n");
        }
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
                serial_spi.putc(pc.getc());
            }
        }
    }
    
    int main()
    {
        pc.printf("\nHello World!\n");
        initWifly();
        terminal();
    
    }
