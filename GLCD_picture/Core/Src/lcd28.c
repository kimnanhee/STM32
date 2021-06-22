#include "stm32f1xx_hal.h"
#include "lcd28.h"
#include "stdlib.h"
#include "lcd.h" 

#define LCD28_DBG_LOG

extern const unsigned char asc2_1206[95][12];
extern const unsigned char asc2_1608[95][16];
extern const unsigned char icon_demo[63720];
extern const unsigned char BIG_ASCII[195][16];

extern unsigned int  POINT_COLOR;
extern unsigned int  BACK_COLOR;


extern u16 lcd_id;	 

void bsp_lcd28_init(void)
{
	

	bsp_lcd_gpio_init();

	delay_ms(10);

	LCD_CS_HIGH();

	// LCD Data(0 ~ 7)
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14| GPIO_PIN_15 |GPIO_PIN_0 | GPIO_PIN_1, GPIO_PIN_SET);  	 
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10,GPIO_PIN_SET);   

	
	LCD_RESET_LOW();

	LCD_RD_HIGH();
	LCD_WR_HIGH();

	delay_ms(10);
	//bsp_fsmc_lcd_init();
	delay_ms(10);

	lcd_rst();

	delay_ms(20);
	delay_ms(20);
	delay_ms(20);
	delay_ms(20);

	LCD_PWM_LIGHT_HIGH();

#ifdef LCD28_DBG_LOG
	usart1_transmit_string("\r\nlcd_rst()\r\n");
#endif

	delay_ms(5); 
	lcd_wr_reg_value(0x0000,0x0001);
	delay_ms(10); 
	
	lcd_wr_reg(0x0000);
	lcd_id = lcd_rd_data();
	
	usart1_transmit_string_format("LCD ID:0x%x \n", lcd_id);  
	
	if(lcd_id==0x9220 )	// ILI9220
	{
		lcd_wr_reg_value(0x0001,0x001B);
		lcd_wr_reg_value(0x0002,0x0700);
		lcd_wr_reg_value(0x0003,0x1030);
		lcd_wr_reg_value(0x0004,0x0000);     
		lcd_wr_reg_value(0x0005,0x0000);
		lcd_wr_reg_value(0x0007,0x0004);      
		lcd_wr_reg_value(0x0008,0x0202);
		lcd_wr_reg_value(0x0009,0x0000);
		lcd_wr_reg_value(0x000B,0x0004);  
		lcd_wr_reg_value(0x000C,0x0003);
		lcd_wr_reg_value(0x000B,0x0004);
		lcd_wr_reg_value(0x0021,0x0000);
		lcd_wr_reg_value(0x0040,0x0000);
		lcd_wr_reg_value(0x0041,0x00EF);
		lcd_wr_reg_value(0x0042,0xDB00);
		lcd_wr_reg_value(0x0043,0xDB00);
		lcd_wr_reg_value(0x0044,0xAF00);
		lcd_wr_reg_value(0x0045,0xDB00);
		delay_ms(10);
		
		//################# void Gamma_Set(void) ####################//
		lcd_wr_reg_value(0x0030,0x0000);  
		lcd_wr_reg_value(0x0031,0x0704);  
		lcd_wr_reg_value(0x0032,0x0004);  
		lcd_wr_reg_value(0x0033,0x0604);  
		lcd_wr_reg_value(0x0034,0x0307);  
		lcd_wr_reg_value(0x0035,0x0103);  
		lcd_wr_reg_value(0x0036,0x0707);  
		lcd_wr_reg_value(0x0037,0x0603);  
		lcd_wr_reg_value(0x0038,0x0000);  
		lcd_wr_reg_value(0x0039,0x0000);
		//############# void Power_Set(void) ################//
		lcd_wr_reg_value(0x0010,0x0000);
		lcd_wr_reg_value(0x0011,0x0000);
		lcd_wr_reg_value(0x0013,0x1413);
		lcd_wr_reg_value(0x0012,0x0008);
		delay_ms(10);
		lcd_wr_reg_value(0x0012,0x0018);
		delay_ms(10);
		lcd_wr_reg_value(0x0010,0x0140);
		delay_ms(20);
		lcd_wr_reg_value(0x0013,0x3414);
		delay_ms(10);
		
		lcd_wr_reg_value(0x0010,0x2140);
		
		//################## void Display_ON(void) ####################//
		lcd_wr_reg_value(0x0010,0x4040);
		lcd_wr_reg_value(0x0007,0x0005);
		delay_ms(4);
		lcd_wr_reg_value(0x0007,0x0025);
		lcd_wr_reg_value(0x0007,0x0027);
		delay_ms(4);
		lcd_wr_reg_value(0x0007,0x0037);
		delay_ms(1);

	}
	else if(lcd_id==0x9325||lcd_id==0x9328)//ILI9325
	{
		lcd_wr_reg_value(0x00e7,0x0010);      
		lcd_wr_reg_value(0x0000,0x0001); // opening internal clock
		lcd_wr_reg_value(0x0001,0x0100);     
		lcd_wr_reg_value(0x0002,0x0700); // power source opening                    
		//lcd_wr_reg_value (0x0003,(1<<3)|(1<<4)); 	//65K RGB
		//DRIVE TABLE (register 03H)
		//BIT3 =AM BIT4:5=ID0:1
		//AM ID0 ID1 FUNCATION
		// 0 0 0 R->L D->U
		// 1 0 0 D->U R->L
		// 0 1 0 L->R D->U
		// 1 1 0 D->U L->R
		// 0 0 1 R->L U->D
		// 1 0 1 U->D R->L
		// 0 1 1 L->R U->D normal uses this.  	
		// 1 1 1 U->D L->R
        lcd_wr_reg_value(0x0003,(1<<12)|(3<<4)|(0<<3)); //65K    
        lcd_wr_reg_value(0x0004,0x0000);                                   
        lcd_wr_reg_value(0x0008,0x0207);	           
        lcd_wr_reg_value(0x0009,0x0000);         
        lcd_wr_reg_value(0x000a,0x0000); //display setting         
        lcd_wr_reg_value(0x000c,0x0001); //display setting          
        lcd_wr_reg_value(0x000d,0x0000); //0f3c          
        lcd_wr_reg_value(0x000f,0x0000);
		// power source disposition
        lcd_wr_reg_value(0x0010,0x0000);   
        lcd_wr_reg_value(0x0011,0x0007);
        lcd_wr_reg_value(0x0012,0x0000);                                                                 
        lcd_wr_reg_value(0x0013,0x0000);                 
        delay_us(5); 
        lcd_wr_reg_value(0x0010,0x1590);   
        lcd_wr_reg_value(0x0011,0x0227);
        delay_us(5); 
        lcd_wr_reg_value(0x0012,0x009c);                  
        delay_us(5); 
        lcd_wr_reg_value(0x0013,0x1900);   
        lcd_wr_reg_value(0x0029,0x0023);
        lcd_wr_reg_value(0x002b,0x000e);
        delay_us(5); 
        lcd_wr_reg_value(0x0020,0x0000);                                                            
        lcd_wr_reg_value(0x0021,0x013f);           
		delay_us(5); 
		// gamma adjustment
        lcd_wr_reg_value(0x0030,0x0007); 
        lcd_wr_reg_value(0x0031,0x0707);   
        lcd_wr_reg_value(0x0032,0x0006);
        lcd_wr_reg_value(0x0035,0x0704);
        lcd_wr_reg_value(0x0036,0x1f04); 
        lcd_wr_reg_value(0x0037,0x0004);
        lcd_wr_reg_value(0x0038,0x0000);        
        lcd_wr_reg_value(0x0039,0x0706);     
        lcd_wr_reg_value(0x003c,0x0701);
        lcd_wr_reg_value(0x003d,0x000f);
        delay_us(5); 
        lcd_wr_reg_value(0x0050,0x0000); // level GRAM reference 
        lcd_wr_reg_value(0x0051,0x00ef); // level GRAM terminated the position                    
        lcd_wr_reg_value(0x0052,0x0000); // vertical GRAM reference                    
        lcd_wr_reg_value(0x0053,0x013f); // vertical GRAM terminated the position  
        
        lcd_wr_reg_value(0x0060,0xa700);        
        lcd_wr_reg_value(0x0061,0x0001); 
        lcd_wr_reg_value(0x006a,0x0000);
        lcd_wr_reg_value(0x0080,0x0000);
        lcd_wr_reg_value(0x0081,0x0000);
        lcd_wr_reg_value(0x0082,0x0000);
        lcd_wr_reg_value(0x0083,0x0000);
        lcd_wr_reg_value(0x0084,0x0000);
        lcd_wr_reg_value(0x0085,0x0000);
      
        lcd_wr_reg_value(0x0090,0x0010);     
        lcd_wr_reg_value(0x0092,0x0000);  
        lcd_wr_reg_value(0x0093,0x0003);
        lcd_wr_reg_value(0x0095,0x0110);
        lcd_wr_reg_value(0x0097,0x0000);        
        lcd_wr_reg_value(0x0098,0x0000);  
        // opens the demonstration establishment    
        lcd_wr_reg_value(0x0007,0x0133);   
        lcd_wr_reg_value(0x0020,0x0000);                                                            
        lcd_wr_reg_value(0x0021,0x013f);
	} 
	else if(lcd_id==0x9320 || lcd_id==0x9300)
	{
		lcd_wr_reg_value(0x00,0x0001);

		HAL_Delay(50);
		
		lcd_wr_reg_value(0x01,0x0100);	//Driver Output Contral.
		lcd_wr_reg_value(0x02,0x0700);	//LCD Driver Waveform Contral.
		HAL_Delay(20); 

		lcd_wr_reg_value(0x03,0x1030); //Entry Mode Set.
		
		HAL_Delay(20); 
	
		lcd_wr_reg_value(0x04,0x0000);	//Scalling Contral.
		lcd_wr_reg_value(0x08,0x0202);	//Display Contral 2. (0x0207)
		lcd_wr_reg_value(0x09,0x0000);	//Display Contral 3. (0x0000)
		lcd_wr_reg_value(0x0a,0x0000);	//Frame Cycle Contal. (0x0000)
		lcd_wr_reg_value(0x0c,(1<<0));	//Extern Display Interface Contral 1. (0x0000)
		lcd_wr_reg_value(0x0d,0x0000);	//Frame Maker Position.
		lcd_wr_reg_value(0x0f,0x0000);	//Extern Display Interface Contral 2.	    

		HAL_Delay(20); 
		lcd_wr_reg_value(0x07,0x0101);	//Display Contral.
		HAL_Delay(20); 								  

		lcd_wr_reg_value(0x10,(1<<12)|(0<<8)|(1<<7)|(1<<6)|(0<<4));	//Power Control 1. (0x16b0)
		HAL_Delay(20); 
		lcd_wr_reg_value(0x11,0x0007);								//Power Control 2. (0x0001)
		HAL_Delay(40); 
		lcd_wr_reg_value(0x12,(1<<8)|(1<<4)|(0<<0));				//Power Control 3. (0x0138)
		HAL_Delay(40); 
		lcd_wr_reg_value(0x13,0x0b00);								//Power Control 4.
		HAL_Delay(20); 
		lcd_wr_reg_value(0x29,0x0000);								//Power Control 7.
		HAL_Delay(20); 
	
		lcd_wr_reg_value(0x2b,(1<<14)|(1<<4));	    
		lcd_wr_reg_value(0x50,0);	//Set X Star
		// level GRAM terminated position Set X End.
		lcd_wr_reg_value(0x51,239);	//Set Y Star
		lcd_wr_reg_value(0x52,0);	//Set Y End.t.
		lcd_wr_reg_value(0x53,319);	//
		HAL_Delay(20); 
	
		lcd_wr_reg_value(0x60,0x2700);	//Driver Output Control.
		lcd_wr_reg_value(0x61,0x0001);	//Driver Output Control.
		lcd_wr_reg_value(0x6a,0x0000);	//Vertical Srcoll Control.
		HAL_Delay(20); 
	
		lcd_wr_reg_value(0x80,0x0000);	//Display Position? Partial Display 1.
		lcd_wr_reg_value(0x81,0x0000);	//RAM Address Start? Partial Display 1.
		lcd_wr_reg_value(0x82,0x0000);	//RAM Address End-Partial Display 1.
		lcd_wr_reg_value(0x83,0x0000);	//Displsy Position? Partial Display 2.
		lcd_wr_reg_value(0x84,0x0000);	//RAM Address Start? Partial Display 2.
		lcd_wr_reg_value(0x85,0x0000);	//RAM Address End? Partial Display 2.
		HAL_Delay(20); 
	
		lcd_wr_reg_value(0x90,(0<<7)|(16<<0));	//Frame Cycle Contral. (0x0013)
		lcd_wr_reg_value(0x92,0x0000);	//Panel Interface Contral 2. (0x0000)
		lcd_wr_reg_value(0x93,0x0001);	//Panel Interface Contral 3.
		lcd_wr_reg_value(0x95,0x0110);	//Frame Cycle Contral. (0x0110)
		lcd_wr_reg_value(0x97,(0<<8));	//
		lcd_wr_reg_value(0x98,0x0000);	//Frame Cycle Contral.	   

		lcd_wr_reg_value(0x07,0x0173);	// (0x0173)
		HAL_Delay(40);
	} 

	
#if 0
	lcd_wr_reg_value(0x00E3,0x3008);        
	lcd_wr_reg_value(0x00E7,0x0012);
	lcd_wr_reg_value(0x00Ef,0x1231); 	// Set the internal timing;    
	//initializing funciton 1 
         lcd_wr_reg_value(0x00,0x0001);
         lcd_wr_reg_value(0x01, 0x0000); // set SS and SM bit
         lcd_wr_reg_value(0x02, 0x0700); // set 1 line inversion
        // lcd_wr_reg_value(0x03, 0x10B0); // set GRAM write direction and BGR=1.
#if   ID_AM==000       
	     lcd_wr_reg_value(0x0003,0x1000);
#elif ID_AM==001        
	     lcd_wr_reg_value(0x0003,0x1008);      
#elif ID_AM==010  
	     lcd_wr_reg_value(0x0003,0x1010);        
#elif ID_AM==011
	     lcd_wr_reg_value(0x0003,0x1018);
#elif ID_AM==100  
	     lcd_wr_reg_value(0x0003,0x1020);      
#elif ID_AM==101  
	     lcd_wr_reg_value(0x0003,0x1028);      
#elif ID_AM==110  
	     lcd_wr_reg_value(0x0003,0x1030);      
#elif ID_AM==111  
	     lcd_wr_reg_value(0x0003,0x1038);
#endif    
         lcd_wr_reg_value(0x04, 0x0000); // Resize register
         lcd_wr_reg_value(0x08, 0x0404); // set the back porch and front porch
         lcd_wr_reg_value(0x09, 0x0000); // set non-display area refresh cycle ISC[3:0]
         lcd_wr_reg_value(0x0A, 0x0000); // FMARK function
        // lcd_wr_reg_value(0x0C, 0x0000); // RGB interface setting
        // lcd_wr_reg_value(0x0D, 0x0000); // Frame marker Position
        // lcd_wr_reg_value(0x0F, 0x0000); // RGB interface polarity
//Power On sequence //
         lcd_wr_reg_value(0x10, 0x0080); // SAP, BT[3:0], AP, DSTB, SLP, STB
         lcd_wr_reg_value(0x11, 0x0007); // DC1[2:0], DC0[2:0], VC[2:0]
         lcd_wr_reg_value(0x12, 0x0000); // VREG1OUT voltage
         lcd_wr_reg_value(0x13, 0x0000); // VDV[4:0] for VCOM amplitude
	 lcd_wr_reg_value(0x07, 0x0001); // 
		 
         delay_us(10); // Dis-charge capacitor power voltage
         
         lcd_wr_reg_value(0x10, 0x1590); // SAP, BT[3:0], AP, DSTB, SLP, STB
         lcd_wr_reg_value(0x11, 0x0227); // DC1[2:0], DC0[2:0], VC[2:0]
         delay_us(5); // delay_us 50ms
        
         lcd_wr_reg_value(0x12, 0x001c); // Internal reference voltage= Vci;
         delay_us(5); // delay_us 50ms
         lcd_wr_reg_value(0x13, 0x1500); // Set VDV[4:0] for VCOM amplitude
         lcd_wr_reg_value(0x29, 0x0010); // Set VCM[5:0] for VCOMH
         lcd_wr_reg_value(0x2B, 0x000f); // Set Frame Rate
         delay_us(5); // delay_us 50ms
         
#if   ID_AM==000         
	     lcd_wr_reg_value(0x0020,0x00ef);
	     lcd_wr_reg_value(0x0021,0x013f);      
#elif ID_AM==001
	     lcd_wr_reg_value(0x0020,0x00ef);
	     lcd_wr_reg_value(0x0021,0x013f);      
#elif ID_AM==010
	     lcd_wr_reg_value(0x0020,0x0000);
	     lcd_wr_reg_value(0x0021,0x013f);      
#elif ID_AM==011
	     lcd_wr_reg_value(0x0020,0x0000);
	     lcd_wr_reg_value(0x0021,0x013f);       
#elif ID_AM==100
	     lcd_wr_reg_value(0x0020,0x00ef);
	     lcd_wr_reg_value(0x0021,0x0000);      
#elif ID_AM==101  
	     lcd_wr_reg_value(0x0020,0x00ef);
	     lcd_wr_reg_value(0x0021,0x0000);      
#elif ID_AM==110
	     lcd_wr_reg_value(0x0020,0x0000);
	     lcd_wr_reg_value(0x0021,0x0000);      
#elif ID_AM==111
	     lcd_wr_reg_value(0x0020,0x0000);
	     lcd_wr_reg_value(0x0021,0x0000);         
#endif  
// ----------- Adjust the Gamma Curve ----------//
		lcd_wr_reg_value(0x0030,0x0101);
		lcd_wr_reg_value(0x0031,0x0707);
		lcd_wr_reg_value(0x0032,0x0505);
		lcd_wr_reg_value(0x0035,0x0107);
		lcd_wr_reg_value(0x0036,0x0108);
		lcd_wr_reg_value(0x0037,0x0102);
		lcd_wr_reg_value(0x0038,0x0202);
		lcd_wr_reg_value(0x0039,0x0106);
		lcd_wr_reg_value(0x003C,0x0202);
		lcd_wr_reg_value(0x003D,0x0806);
//------------------ Set GRAM area ---------------//
         lcd_wr_reg_value(0x0050, 0x0000); // Horizontal GRAM Start Address
         lcd_wr_reg_value(0x0051, 0x00EF); // Horizontal GRAM End Address
         lcd_wr_reg_value(0x0052, 0x0000); // Vertical GRAM Start Address
         lcd_wr_reg_value(0x0053, 0x013F); // Vertical GRAM Start Address
         lcd_wr_reg_value(0x0060, 0xA700); // Gate Scan Line
         lcd_wr_reg_value(0x0061, 0x0001); // NDL,VLE, REV
         lcd_wr_reg_value(0x006A, 0x0000); // set scrolling line
//-------------- Partial Display Control ---------//
         lcd_wr_reg_value(0x80, 0x0000);
         lcd_wr_reg_value(0x81, 0x0000);
         lcd_wr_reg_value(0x82, 0x0000);
         lcd_wr_reg_value(0x83, 0x0000);
         lcd_wr_reg_value(0x84, 0x0000);
         lcd_wr_reg_value(0x85, 0x0000);
//-------------- Panel Control -------------------//
         lcd_wr_reg_value(0x90, 0x0010);
         lcd_wr_reg_value(0x92, 0x0000);
         lcd_wr_reg_value(0x93, 0x0003);
         lcd_wr_reg_value(0x95, 0x0110);
         lcd_wr_reg_value(0x97, 0x0000);
         lcd_wr_reg_value(0x98, 0x0000);
         lcd_wr_reg_value(0x07, 0x0173); // 262K color and display ON   0133

		 HAL_Delay(5);

	}	
#endif
	 

	// 2.8 lcd backlight on
	LCD_PWM_LIGHT_LOW(); //--> 2.8 LCD original code	

	HAL_Delay(20);

	lcd28_clear_screen(RED);

}


// starts to write GRAM
void lcd28_writeram_prepare(void)
{
	lcd_wr_reg(R34);
}	 

void lcd28_setcursor (u8 Xpos, u16 Ypos)
{
	lcd_wr_reg_value (R32, Xpos);
	lcd_wr_reg_value (R33, Ypos);
} 

// 320 * 240
void lcd28_clear_screen(unsigned int color)
{                    
	u32 index=0;      
	lcd28_setcursor(0x00,0x0000); // establishment cursor position 
	lcd28_writeram_prepare();     // starts to read in GRAM	 	  
	for (index=0; index<320*240; index++)
	{
		lcd_wr_data(color);    
	}
}

void lcd28_draw_point (u16 x, u16 y)
{
	lcd28_setcursor(x, y); // establishment cursor position 
	lcd_wr_reg(R34); // starts to read in GRAM
	lcd_wr_data(POINT_COLOR); 
} 	

void lcd28_fill_color(u8 xsta, u16 ysta, u8 xend, u16 yend, u16 color)
{                    
    u32 n;
	// establishment window										
	lcd_wr_reg_value (R80, xsta); // horizontal direction GRAM start address
	lcd_wr_reg_value (R81, xend); // horizontal direction GRAM end address
	lcd_wr_reg_value (R82, ysta); // vertical direction GRAM start address
	lcd_wr_reg_value (R83, yend); // vertical direction GRAM end address	
	lcd28_setcursor(xsta, ysta); // establishment cursor position  
	lcd28_writeram_prepare();  // starts to read in GRAM	 	   	   
	n=(u32)(yend-ysta+1)*(xend-xsta+1);    
	while (n--) {lcd_wr_data(color);}// demonstration fills color. 
	// resumes the establishment
	lcd_wr_reg_value (R80, 0x0000); // horizontal direction GRAM start address
	lcd_wr_reg_value (R81, 0x00EF); // horizontal direction GRAM end address
	lcd_wr_reg_value (R82, 0x0000); // vertical direction GRAM start address
	lcd_wr_reg_value (R83, 0x013F); // vertical direction GRAM end address	    
}  

// draws a line
//x1, y1: Beginning coordinates
//x2, y2: End point coordinates  
void lcd28_draw_line (u16 x1, u16 y1, u16 x2, u16 y2)
{
	u16 t; 
	int xerr=0, yerr=0, delta_x, delta_y, distance; 
	int incx, incy, uRow, uCol; 

	delta_x=x2-x1; // computation increase of coordinates 
	delta_y=y2-y1; 
	uRow=x1; 
	uCol=y1; 
	if(delta_x>0)incx=1; // establishes on foot the direction 
	else if(delta_x==0)incx=0; // perpendicular line 
	else {incx=-1; delta_x=-delta_x;} 
	if(delta_y>0)incy=1; 
	else if(delta_y==0)incy=0; // level line 
	else {incy=-1; delta_y=-delta_y;} 
	if (delta_x>delta_y) distance=delta_x; // selects the basic increase coordinate axis 
	else distance=delta_y; 
	for (t=0; t<=distance+1; t++) // linedraw output 
	{  
		lcd28_draw_point(uRow, uCol); // picture spot 
		xerr+=delta_x; 
		yerr+=delta_y; 
		if(xerr>distance) 
		{ 
			xerr-=distance; 
			uRow+=incx; 
		} 
		if(yerr>distance) 
		{ 
			yerr-=distance; 
			uCol+=incy; 
		} 
	}  
}    
// picture rectangle
void lcd28_draw_rectangle (u8 x1, u16 y1, u8 x2, u16 y2)
{
	lcd28_draw_line(x1, y1, x2, y1);
	lcd28_draw_line(x1, y1, x1, y2);
	lcd28_draw_line(x1, y2, x2, y2);
	lcd28_draw_line(x2, y1, x2, y2);
}
// is assigning the position to draw one to assign the size the circle
// (x, y): Central point
//r: Radius
void lcd28_draw_circle(u8 x0, u16 y0, u8 r)
{
	int a, b;
	int di;
	a=0; b=r;	  
	di=3-(r<<1);             // judgment spot position symbol
	while(a<=b)
	{
		lcd28_draw_point(x0-b, y0-a);             //3           
		lcd28_draw_point(x0+b, y0-a);             //0           
		lcd28_draw_point(x0-a, y0+b);             //1       
		lcd28_draw_point(x0-b, y0-a);             //7           
		lcd28_draw_point(x0-a, y0-b);             //2             
		lcd28_draw_point(x0+b, y0+a);             //4               
		lcd28_draw_point(x0+a, y0-b);             //5
		lcd28_draw_point(x0+a, y0+b);             //6 
		lcd28_draw_point(x0-b, y0+a);             
		a++;
		// uses the Bresenham algorithm to make a circle     
		if(di<0)di +=4*a+6;	  
		else
		{
			di+=10+4*(a-b);   
			b--;
		} 
		lcd28_draw_point(x0+a, y0+b);
	}
} 
// is assigning the position to demonstrate a character
//x: 0~234
//y: 0~308
//num: Must demonstrate character: ""---> " ~"
//size: Font size 12/16
//mode: The superimposition way (1) is also the non-superimposition way (0)
void lcd28_show_char (u8 x, u16 y, u8 num, u8 size, u8 mode)
{       

#define MAX_CHAR28_POSX 232
#define MAX_CHAR28_POSY 304 

    u8 temp;
    u8 pos, t;      

    if(x>MAX_CHAR28_POSX || y>MAX_CHAR28_POSY)
		return;	    

	// establishment window										
	lcd_wr_reg_value(R80, x);           // horizontal direction GRAM start address
	lcd_wr_reg_value(R81, x+(size/2-1)); // horizontal direction GRAM end address
	lcd_wr_reg_value(R82, y);           // vertical direction GRAM start address
	lcd_wr_reg_value(R83, y+size-1);    // vertical direction GRAM end address	
	lcd28_setcursor(x, y);            // establishment cursor position  
	lcd28_writeram_prepare();        // starts to read in GRAM	   
	num = num - ' '; // obtains the displacement value
	if (!mode) // non-superimposition way
	{
		for (pos=0; pos<size; pos++)
		{
			if(size==12)temp=asc2_1206[num][pos]; // transfers 1206 typefaces
			else temp=asc2_1608[num][pos];		 // transfers 1608 typefaces
			for (t=0; t<size/2; t++)
		    {                 
		        if(temp&0x01)
				{
					lcd_wr_data(POINT_COLOR);
				} else lcd_wr_data(BACK_COLOR);	        
		        temp>>=1; 
		    }
		}	
	} else//superimposition way
	{
		for (pos=0; pos<size; pos++)
		{
			if(size==12)temp=asc2_1206[num][pos]; // transfers 1206 typefaces
			else temp=asc2_1608[num][pos];		 // transfers 1608 typefaces
			for (t=0; t<size/2; t++)
		    {                 
		        if(temp&0x01)lcd28_draw_point(x+t, y+pos); // draws a spot     
		        temp>>=1; 
		    }
		}
	}	    
	// restores the window size	 
	lcd_wr_reg_value (R80, 0x0000); // horizontal direction GRAM start address
	lcd_wr_reg_value (R81, 0x00EF); // horizontal direction GRAM end address
	lcd_wr_reg_value (R82, 0x0000); // vertical direction GRAM start address
	lcd_wr_reg_value (R83, 0x013F); // vertical direction GRAM end address
}  


// demonstrates 2 numerals
//x, y: Beginning coordinates	 
//len: Digital figure
//size: Font size
//color: Color
//num: Value (0~4294967295);	 
void lcd28_show_num(u8 x, u16 y, u32 num, u8 len, u8 size)
{         	
	u8 t, temp;
	u8 enshow=0;						   
	for (t=0; t<len; t++)
	{
		temp=(num/mypow(10, len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				lcd28_show_char(x+(size/2) *t, y, ' ', size,0);
				continue;
			} else enshow=1; 
		 	 
		}
	 	lcd28_show_char(x+(size/2) *t, y, temp+'0', size,0); 
	}
} 

// demonstration string of character
//x, y: Beginning coordinates  
// *p: String of character start address
// uses 16 typefaces
void lcd28_show_string (u8 x, u16 y, const u8 *p)
{         
    while (*p != '\0')
    {       
        if(x>MAX_CHAR28_POSX) 
		{
			x=0; 
			y+=16;
		}
        
		if(y>MAX_CHAR28_POSY) 
		{
			y=x=0; 
			lcd28_clear_screen(WHITE);
		}

        lcd28_show_char(x, y,*p,16,0);
        x+=8;
        p++;
    }  
}

void lcd28_draw_point_big(u16 x,u16 y)
{
	lcd28_draw_point(x,y);
	lcd28_draw_point(x+1,y);
	lcd28_draw_point(x,y+1);
	lcd28_draw_point(x+1,y+1);
}
