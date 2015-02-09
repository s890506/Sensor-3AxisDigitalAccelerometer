/*
 * MMA7760.h
 * Library for accelerometer_MMA7760
 *
 * Copyright (c) 2013 seeed technology inc.
 * Author        :   FrankieChu
 * Create Time   :   Jan 2013
 * Change Log    :
 *
 * The MIT License (MIT)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "MMA7660.h"

I2C i2c(I2C_SDA,I2C_SCL);

/*Function: Write a byte to the register of the MMA7660*/
void MMA7660::write(uint8_t _register, uint8_t _data)
{
    char data[2] = {_register,_data};
    int check;
    //Wire.begin();
    //Wire.beginTransmission(MMA7660_ADDR);
    //Wire.write(_register);
    //Wire.write(_data);
    //Wire.endTransmission();
    check = i2c.write(MMA7660_ADDR<<1,data,2);
    if(check != 0)
        printf("I2C Write Failure\n\r");
}
/*Function: Read a byte from the regitster of the MMA7660*/
uint8_t MMA7660::read(uint8_t _register)
{
    char data_read[2] = {_register,0};
    int check;
    check = i2c.write(MMA7660_ADDR<<1,data_read,1); // write _register to set up read
    check = i2c.read(MMA7660_ADDR<<1,data_read,2); // execute read
    if(check != 0)
        printf("I2C Read failure\n\r");
    printf("read back data 0x%x, 0x%x from register 0x%x",data_read[0],data_read[1],_register);
    //uint8_t data_read;
    //Wire.begin();
    //Wire.beginTransmission(MMA7660_ADDR);
    //Wire.write(_register);
    //Wire.endTransmission();
    //Wire.beginTransmission(MMA7660_ADDR);
    //Wire.requestFrom(MMA7660_ADDR,1);
    //while(Wire.available())
    //{
    //  data_read = Wire.read();
    //}
    //Wire.endTransmission();
    return data_read[1];
}

void MMA7660::init()
{
    i2c.frequency(400000);
    setMode(MMA7660_STAND_BY);
    setSampleRate(AUTO_SLEEP_32);
    setMode(MMA7660_ACTIVE);
}
void MMA7660::setMode(uint8_t mode)
{
    write(MMA7660_MODE,mode);
}
void MMA7660::setSampleRate(uint8_t rate)
{
    write(MMA7660_SR,rate);
}
/*Function: Get the contents of the registers in the MMA7660*/
/*          so as to calculate the acceleration.            */
void MMA7660::getXYZ(int8_t *x,int8_t *y,int8_t *z)
{
    char val[3];
    int count = 0;
    bool done = false;
    val[0] = val[1] = val[2] = 64;
//    while(Wire.available() > 0)
//        Wire.read();
//    Wire.requestFrom(MMA7660_ADDR,3);
//    while(Wire.available())
//    {
//        if(count < 3)
//        {
//            while ( val[count] > 63 )  // reload the damn thing it is bad
//            {
//              val[count] = Wire.read();
//            }
//        }
//        count++;
//    }
    for(count = 0; count < 3 && done == false; count ++){
        i2c.read(MMA7660_ADDR<<1, val,3);
        if(val[0] < 63 && val[1]<63 && val[2]<63)
            done = true;
    }
    
    *x = ((char)(val[0]<<2))/4;
    *y = ((char)(val[1]<<2))/4;
    *z = ((char)(val[2]<<2))/4;
}

void MMA7660::getAcceleration(float *ax,float *ay,float *az)
{
    int8_t x,y,z;
    getXYZ(&x,&y,&z);
    *ax = x/21.00;
    *ay = y/21.00;
    *az = z/21.00;
}
