#include <stdio.h>
#include <stdlib.h>
#include<string.h>

//Sbox 
unsigned char s_box[16] = {
  0x6,0xb,0x5,0x4,0x2,0xe,0x7,0xa,0x9,0xd,0xf,0xc,0x3,0x1,0x0,0x8
};

//Inverse Sbox
unsigned char inverse_s_box[16] = {
  0xe,0xd,0x4,0xc,0x3,0x2,0x0,0x6,0xf,0x8,0x7,0x1,0xb,0x9,0x5,0xa
};

//Mul 2
unsigned char mul2[16] = {
  0x0,0x2,0x4,0x6,0x8,0xa,0xc,0xe,0x3,0x1,0x7,0x5,0xb,0x9,0xf,0xd
};

//Mul 3
unsigned char mul3[16] = {
  0x0,0x3,0x6,0x5,0xc,0xf,0xa,0x9,0xb,0x8,0xd,0xe,0x7,0x4,0x1,0x2
};

//Mul 9
unsigned char mul9[16]={
    0x0,0x9,0x1,0x8,0x2,0xb,0x3,0xa,0x4,0xd,0x5,0xc,0x6,0xf,0x7,0xe
};

//Mul 11
unsigned char mul11[16]={
    0x0,0xb,0x5,0xe,0xa,0x1,0xf,0x4,0x7,0xc,0x2,0x9,0xd,0x6,0x8,0x3
};

//Mul 13
unsigned char mul13[16]={
    0x0,0xd,0x9,0x4,0x1,0xc,0x8,0x5,0x2,0xf,0xb,0x6,0x3,0xe,0xa,0x7
};

//Mul 14
unsigned char mul14[16]={
    0x0,0xe,0xf,0x1,0xd,0x3,0x2,0xc,0x9,0x7,0x6,0x8,0x4,0xa,0xb,0x5
};

//Round counter
unsigned char rcon[11] = {
  0x1,0x2,0x4,0x8,0x3,0x6,0xc,0xb,0x5,0xa,0x7
};

//KeyExpansionCore
void KeyExpansionCore(unsigned char* in, unsigned char i)
{
 //rotate left 
 unsigned char t=in[0];
 in[0]=in[1];
 in[1]=in[2];
 in[2]=in[3];
 in[3]=t;
 //S-box four nibbles
 in[0]=s_box[in[0]];
 in[1]=s_box[in[1]];
 in[2]=s_box[in[2]];
 in[3]=s_box[in[3]];
 //Rcon
 in[0] ^= rcon[i-1];
}

//KeyExpansion
void KeyExpansion(unsigned char* inputkey, unsigned char* expandedkeys)
{
 //first 16 nibble are original keys
 for(int i=0;i<16;i++)
    expandedkeys[i]=inputkey[i];
 
 //variables
 int nibblesGenerated=16;
 int rconIneration =1;
 unsigned char temp[4];

 while(nibblesGenerated<176)
  {
   //read 4 nibble for the core
   for(int i=0;i<4;i++)
      temp[i]=expandedkeys[i+nibblesGenerated-4];
   
   //perform the core once for each 16 nibble key;
   if(nibblesGenerated%16==0)
     {
       KeyExpansionCore(temp, rconIneration);
       rconIneration++;
     }
    //XOR temp with [nibblesGenerated-16], and store in expandedkeys:
    for(unsigned char a=0;a<4;a++)
    {
      expandedkeys[nibblesGenerated]=expandedkeys[nibblesGenerated-16]^temp[a];
      nibblesGenerated++;
    }
  }
}

//SubBytes
void SubBytes(unsigned char* state)
{
    for(int i=0;i<16;i++)
    {
        state[i]=s_box[state[i]];
    }
}

//ShiftRows
void ShiftRows(unsigned char* state)
{
 unsigned char tmp[16];
 tmp[0]=state[0];
 tmp[1]=state[5];
 tmp[2]=state[10];
 tmp[3]=state[15];
 
 tmp[4]=state[4];
 tmp[5]=state[9];
 tmp[6]=state[14];
 tmp[7]=state[3];
 
 tmp[8]=state[8];
 tmp[9]=state[13];
 tmp[10]=state[2];
 tmp[11]=state[7];

 tmp[12]=state[12];
 tmp[13]=state[1];
 tmp[14]=state[6];
 tmp[15]=state[11];

 for(int i=0;i<16;i++)
    state[i]=tmp[i];
}

// Inverse Shifts rows right 
void InverseShiftRows(unsigned char * state) {
	unsigned char tmp[16];

	/* Column 1 */
	tmp[0] = state[0];
	tmp[1] = state[13];
	tmp[2] = state[10];
	tmp[3] = state[7];

	/* Column 2 */
	tmp[4] = state[4];
	tmp[5] = state[1];
	tmp[6] = state[14];
	tmp[7] = state[11];

	/* Column 3 */
	tmp[8] = state[8];
	tmp[9] = state[5];
	tmp[10] = state[2];
	tmp[11] = state[15];

	/* Column 4 */
	tmp[12] = state[12];
	tmp[13] = state[9];
	tmp[14] = state[6];
	tmp[15] = state[3];

	for ( int i = 0; i < 16; i++) {
		state[i] = tmp[i];             
	}
}

//Inverse MixColumns
void InverseMixColumns(unsigned char* state) 
{
	unsigned char tmp[16];

 tmp[0] = (unsigned char)mul14[state[0]] ^ mul11[state[1]] ^ mul13[state[2]] ^ mul9[state[3]];
 tmp[1] = (unsigned char)mul9[state[0]] ^ mul14[state[1]] ^ mul11[state[2]] ^ mul13[state[3]];
 tmp[2] = (unsigned char)mul13[state[0]] ^ mul9[state[1]] ^ mul14[state[2]] ^ mul11[state[3]];
 tmp[3] = (unsigned char)mul11[state[0]] ^ mul13[state[1]] ^ mul9[state[2]] ^ mul14[state[3]];

 tmp[4] = (unsigned char)mul14[state[4]] ^ mul11[state[5]] ^ mul13[state[6]] ^ mul9[state[7]];
 tmp[5] = (unsigned char)mul9[state[4]] ^ mul14[state[5]] ^ mul11[state[6]] ^ mul13[state[7]];
 tmp[6] = (unsigned char)mul13[state[4]] ^ mul9[state[5]] ^ mul14[state[6]] ^ mul11[state[7]];
 tmp[7] = (unsigned char)mul11[state[4]] ^ mul13[state[5]] ^ mul9[state[6]] ^ mul14[state[7]];

 tmp[8] = (unsigned char)mul14[state[8]] ^ mul11[state[9]] ^ mul13[state[10]] ^ mul9[state[11]];
 tmp[9] = (unsigned char)mul9[state[8]] ^ mul14[state[9]] ^ mul11[state[10]] ^ mul13[state[11]];
 tmp[10] = (unsigned char)mul13[state[8]] ^ mul9[state[9]] ^ mul14[state[10]] ^ mul11[state[11]];
 tmp[11] = (unsigned char)mul11[state[8]] ^ mul13[state[9]] ^ mul9[state[10]] ^ mul14[state[11]];

 tmp[12] = (unsigned char)mul14[state[12]] ^ mul11[state[13]] ^ mul13[state[14]] ^ mul9[state[15]];
 tmp[13] = (unsigned char)mul9[state[12]] ^ mul14[state[13]] ^ mul11[state[14]] ^ mul13[state[15]];
 tmp[14] = (unsigned char)mul13[state[12]] ^ mul9[state[13]] ^ mul14[state[14]] ^ mul11[state[15]];
 tmp[15] = (unsigned char)mul11[state[12]] ^ mul13[state[13]] ^ mul9[state[14]] ^ mul14[state[15]];

	for (unsigned char i = 0; i < 16; i++) {
		state[i] = tmp[i];
	}
        
}

//MixColumns
void MixColumn(unsigned char* state)
{
    unsigned char tmp[16];

 tmp[0] = (unsigned char)(mul2[state[0]]^mul3[state[1]]^state[2]^state[3]);
 tmp[1] = (unsigned char)(state[0]^mul2[state[1]]^mul3[state[2]]^state[3]);
 tmp[2] = (unsigned char)(state[0]^state[1]^mul2[state[2]]^mul3[state[3]]);
 tmp[3] = (unsigned char)(mul3[state[0]]^state[1]^state[2]^mul2[state[3]]);
 
 tmp[4] = (unsigned char)(mul2[state[4]]^mul3[state[5]]^state[6]^state[7]);
 tmp[5] = (unsigned char)(state[4]^mul2[state[5]]^mul3[state[6]]^state[7]);
 tmp[6] = (unsigned char)(state[4]^state[5]^mul2[state[6]]^mul3[state[7]]);
 tmp[7] = (unsigned char)(mul3[state[4]]^state[5]^state[6]^mul2[state[7]]);
 
 tmp[8] = (unsigned char)(mul2[state[8]]^mul3[state[9]]^state[10]^state[11]);
 tmp[9] = (unsigned char)(state[8]^mul2[state[9]]^mul3[state[10]]^state[11]);
 tmp[10] = (unsigned char)(state[8]^state[9]^mul2[state[10]]^mul3[state[11]]);
 tmp[11] = (unsigned char)(mul3[state[8]]^state[9]^state[10]^mul2[state[11]]);
 
 tmp[12] = (unsigned char)(mul2[state[12]]^mul3[state[13]]^state[14]^state[15]);
 tmp[13] = (unsigned char)(state[12]^mul2[state[13]]^mul3[state[14]]^state[15]);
 tmp[14] = (unsigned char)(state[12]^state[13]^mul2[state[14]]^mul3[state[15]]);
 tmp[15] = (unsigned char)(mul3[state[12]]^state[13]^state[14]^mul2[state[15]]);
 
 for(int i=0;i<16;i++)
 {state[i]=tmp[i];}
  
}

//AddRoundKey
void AddRoundKey(unsigned char* state, unsigned char* roundKey)
{
    for(int i=0;i<16;i++)
        state[i] ^= roundKey[i];
    
}

//AES_Encrypt
void AES_Encrypt(unsigned char* message, unsigned char* key)
	{
	    unsigned char state[16];//16 nibble
	    for(int i=0;i<16;i++)
	    {
	        state[i]=message[i];
	    }
	    int numberOfRounds=9;
            unsigned char expandedkey[176];
	    KeyExpansion(key,expandedkey);
	    AddRoundKey(state, key);
	    
	    for(int i=0;i<numberOfRounds;i++)
	    {
	        SubBytes(state);
	        ShiftRows(state);
	        MixColumn(state);
	        AddRoundKey(state,expandedkey+(16*(i+1)));
	    }
            
	    //Final Round
	        SubBytes(state);
	        ShiftRows(state);
	        AddRoundKey(state,expandedkey+160);
        for(int i=0;i<16;i++)
        {
          message[i]=state[i];
        }

}

//PrintXex
void PrintHex(unsigned char x)
{

 if(x%16<10) printf("%c",(char)((x%16)+'0'));
 if(x%16>=10) printf("%c",(char)((x%16-10)+'A'));
}

//Main
int main()
{
	 //test vector 1
        unsigned char message1[16]={0x4,0x5,0x8,0xe,0x8,0x7,0x2,0xb,0xa,0x5,0x1,0xe,0xe,0x2,0xa,0xf};//you can change here (Plaintext)
        unsigned char key1[16]={0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1};//you can change here (Key)

             
        printf("Plaintext:  ");
        for(int i=0;i<16;i++)
        { printf("%X",message1[i]);}

        printf("\nKey:        ");
        for(int i=0;i<16;i++)
        { printf("%X",key1[i]);}
        
	AES_Encrypt(message1,key1);//Call Encryption function
        
        printf("\nCiphertext: ");
        for(int i=0;i<16;i++)
        {
          printf("%X",message1[i]);
        }
        printf("\n"); 
        
	return 0;
}


