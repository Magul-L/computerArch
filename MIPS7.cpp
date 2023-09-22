#include <iostream>
#include <string>
#include <vector>
#include <bitset>
#include <fstream>

using namespace std;

#define ADDU (1)
#define SUBU (3)
#define AND (4)
#define OR (5)
#define NOR (7)
int count = 1;
// Memory size.
// In reality, the memory size should be 2^32, but for this lab and space reasons,
// we keep it as this large number, but the memory is still 32-bit addressable.
#define MemSize (65536)
int error = 0;
class RF
{
public:
  bitset<32> ReadData1, ReadData2;
  RF()
  {
    Registers.resize(32);
    Registers[0] = bitset<32>(0);
  }

  void ReadWrite(bitset<5> RdReg1, bitset<5> RdReg2, bitset<5> WrtReg, bitset<32> WrtData, bitset<1> WrtEnable)
  {
    /**
     * @brief Reads or writes data from/to the Register.
     *
     * This function is used to read or write data from/to the register, depending on the value of WrtEnable.
     * Put the read results to the ReadData1 and ReadData2.
     */
    // TODO: implement!
    ReadData1 = Registers[RdReg1.to_ulong()];
    ReadData2 = Registers[RdReg2.to_ulong()];

    if (WrtEnable == 1)
    {
      Registers[WrtReg.to_ulong()] = WrtData;
    }
  }

  void OutputRF()
  {
    ofstream rfout;
    rfout.open("RFresult.txt", std::ios_base::app);
    if (rfout.is_open())
    {
      rfout << "A state of RF:" << endl;
      for (int j = 0; j < 32; j++)
      {
        rfout << Registers[j] << endl;
      }
    }
    else
      cout << "Unable to open file";
    rfout.close();
  }

private:
  vector<bitset<32>> Registers;
};

class ALU
{
public:
  bitset<32> ALUresult;
  bitset<32> ALUOperation(bitset<3> ALUOP, bitset<32> oprand1, bitset<32> oprand2)
  {
    /**
     * @brief Implement the ALU operation here.
     *
     * ALU operation depends on the ALUOP, which are definded as ADDU, SUBU, etc.
     */
    // TODO: implement!

    if (ALUOP == ADDU)
    {
      ALUresult = oprand1.to_ulong() + oprand2.to_ulong();
    }
    else if (ALUOP == SUBU)
    {
      ALUresult = oprand1.to_ulong() - oprand2.to_ulong();
    }
    else if (ALUOP == AND)
    {
      ALUresult = oprand1 & oprand2;
    }
    else if (ALUOP == OR)
    {
      ALUresult = oprand1 | oprand2;
    }
    else if (ALUOP == NOR)
    {
      ALUresult = ~(oprand1 | oprand2);
    }
    else
    {
      std::cerr << "wrong operation" << std::endl;
      ALUresult = 0;
    }

    return ALUresult;
  }
};

class INSMem
{
public:
  bitset<32> Instruction;
  INSMem()
  {
    IMem.resize(MemSize);
    ifstream imem;
    string line;
    int i = 0;
    imem.open("imem.txt");
    if (imem.is_open())
    {
      while (getline(imem, line))
      {
        IMem[i] = bitset<8>(line);
        i++;
      }
    }
    else
      cout << "Unable to open file";
    imem.close();
  }

  bitset<32> ReadMemory(bitset<32> ReadAddress)
  {

    // TODO: implement!
    /**
     * @brief Read Instruction Memory (IMem).
     *
     * Read the byte at the ReadAddress and the following three byte,
     * and return the read result.
     */
    int addr = ReadAddress.to_ulong();
    if (addr >= MemSize)
    {
      cerr << "Error: Memory access out of bounds!" << endl;
      error = 1;
    }



    //bitset<32> instruction(0);
    string byte;
    for (int i = 0; i < 4; i++)
    {
      byte = byte + IMem[addr + i].to_string();
    }
    std::bitset<32> instruction (byte);
    Instruction = instruction;
    return Instruction;
  }

private:
  vector<bitset<8>> IMem;
};

class DataMem
{
public:
  bitset<32> readdata;
  DataMem()
  {
    DMem.resize(MemSize);
    ifstream dmem;
    string line;
    int i = 0;
    dmem.open("dmem.txt");
    if (dmem.is_open())
    {
      while (getline(dmem, line))
      {
        DMem[i] = bitset<8>(line);
        i++;
      }
    }
    else
      cout << "Unable to open file";
    dmem.close();
  }
  
  bitset<32> MemoryAccess(bitset<32> Address, bitset<32> WriteData, bitset<1> readmem, bitset<1> writemem)
  {
    /**
     * @brief Reads/writes data from/to the Data Memory.
     *
     * This function is used to read/write data from/to the DataMem, depending on the readmem and writemem.
     * First, if writemem enabled, WriteData should be written to DMem, clear or ignore the return value readdata,
     * and note that 32-bit WriteData will occupy 4 continious Bytes in DMem.
     * If readmem enabled, return the DMem read result as readdata.
     */
    // TODO: implement!
    int addr = Address.to_ulong();
    string byte = "";
    string wd = WriteData.to_string();

    if (addr >= MemSize)
    {
      cerr << "Error: Memory access out of bounds!" << endl;
      return bitset<32>(0);
    }

    if (writemem == 1)
    {
      for (int i = 0; i < 4; i++)
      {
        
        byte = wd.substr(i*8,8);
        DMem[addr + i] = bitset <8> (byte);
      }
    }

    if (readmem == 1)
    {
      ;
      for (int i = 0; i < 4; i++)
      {
        byte =  byte + DMem[addr+i].to_string();

      }
      readdata = bitset <32> (byte);
    }

    return readdata;
  }

  void OutputDataMem()
  {
    ofstream dmemout;
    dmemout.open("dmemresult.txt");
    if (dmemout.is_open())
    {
      for (int j = 0; j < 1000; j++)
      {
        dmemout << DMem[j] << endl;
      }
    }
    else
      cout << "Unable to open file";
    dmemout.close();
  }

private:
  vector<bitset<8>> DMem;
};

int main()
{

  RF myRF;
  ALU myALU;
  INSMem myInsMem;
  DataMem myDataMem;
  int pc = 0;
  
  bitset<32> end("11111111111111111111111111111111");
  while (1)  // TODO: implement!
  {
    // Fetch: fetch an instruction from myInsMem.

    // If current instruction is "11111111111111111111111111111111", then break; (exit the while loop)

    // decode(Read RF): get opcode and other signals from instruction, decode instruction

    // Execute: after decoding, ALU may run and return result

    // Read/Write Mem: access data memory (myDataMem)

    // Write back to RF: some operations may write things to RF
    bitset<32> fetch = myInsMem.ReadMemory(bitset<32> (pc));
    pc = pc + 4;
    // if(count > 16383){
    //   break;
    // }
    
    if(fetch == bitset<32> ("00000000000000000000000000000000")){
      break;
    }
    if(fetch == end ){

      break;

    }
    //rtype
    if(fetch.to_string().substr(0,6) == "000000"){
      bitset<5> rs(fetch.to_string().substr(6,5));
      bitset<5> rt(fetch.to_string().substr(11,5));
      bitset<5> rd(fetch.to_string().substr(16,5));
      bitset<6> funct(fetch.to_string().substr(26,6));
      bitset<6> addu(0x21);
      bitset<6> subu(0x23);
      bitset<6> And(0x24);
      bitset<6> Or(0x25);
      bitset<6> nor(0x27);

      if(funct.to_string() == addu.to_string()){
        
        myRF.ReadWrite(rs,rt,rd,bitset<32> (0),0);
        myALU.ALUOperation(ADDU,myRF.ReadData1,myRF.ReadData2);
        myRF.ReadWrite(rs,rt,rd,myALU.ALUresult,1);
      
      }

      else if(funct.to_string() == subu.to_string()){
        
        myRF.ReadWrite(rs,rt,rd,bitset<32> (0),0);
        myALU.ALUOperation(SUBU,myRF.ReadData1,myRF.ReadData2);
        myRF.ReadWrite(rs,rt,rd,myALU.ALUresult,1);

      }
      
      else if(funct.to_string() == And.to_string()){
        
        myRF.ReadWrite(rs,rt,rd,bitset<32> (0),0);
        myALU.ALUOperation(AND,myRF.ReadData1,myRF.ReadData2);
        myRF.ReadWrite(rs,rt,rd,myALU.ALUresult,1);

      }
      
      else if(funct.to_string() == Or.to_string()){
        
        myRF.ReadWrite(rs,rt,rd,bitset<32> (0),0);
        myALU.ALUOperation(OR,myRF.ReadData1,myRF.ReadData2);
        myRF.ReadWrite(rs,rt,rd,myALU.ALUresult,1);

      }

      else if(funct.to_string() == nor.to_string()){
        
        myRF.ReadWrite(rs,rt,rd,bitset<32> (0),0);
        myALU.ALUOperation(NOR,myRF.ReadData1,myRF.ReadData2);
        myRF.ReadWrite(rs,rt,rd,myALU.ALUresult,1);

      }
    
    }

    //addiu
    else if(fetch.to_string().substr(0,6) == "001001"){

      bitset<5> rs(fetch.to_string().substr(6,5));
      bitset<5> rt(fetch.to_string().substr(11,5));
      bitset<32> imm(fetch.to_string().substr(16,16));
      
      myRF.ReadWrite(rs,rt,rt,bitset<32> (0),0);
      myALU.ALUOperation(ADDU,myRF.ReadData1, imm);
      myRF.ReadWrite(rs,rt,rt,myALU.ALUresult,1);

    }
    //beq
    else if(fetch.to_string().substr(0,6) == "000100"){

      bitset<5> rs(fetch.to_string().substr(6,5));
      bitset<5> rt(fetch.to_string().substr(11,5));
      bitset<32> imm(fetch.to_string().substr(16,16));

      myRF.ReadWrite(rs,rt,rt,bitset<32> (0),0);
      if(myRF.ReadData1.to_ulong() == myRF.ReadData2.to_ulong()){

        pc = pc + imm.to_ulong();

      }
      

    }
    //lw
    else if(fetch.to_string().substr(0,6) == "100011"){

      bitset<5> rs(fetch.to_string().substr(6,5));
      bitset<5> rt(fetch.to_string().substr(11,5));
      bitset<32> imm(fetch.to_string().substr(16,16));
      bitset<32> addr(0);
      long addr_result;

      myRF.ReadWrite(rs,rt,rt,bitset<32> (0),0);
      addr_result = imm.to_ulong() + myRF.ReadData1.to_ulong();
      addr = bitset<32> (addr_result);
      myDataMem.MemoryAccess(addr,bitset<32> (0),1,0);
      myRF.ReadWrite(rs,rt,rt,myDataMem.readdata,1);
  
    }
    //sw
    else if(fetch.to_string().substr(0,6) == "101011"){
      
      bitset<5> rs(fetch.to_string().substr(6,5));
      bitset<5> rt(fetch.to_string().substr(11,5));
      bitset<32> imm(fetch.to_string().substr(16,16));
      bitset<32> addr(0);
      long addr_result;

      myRF.ReadWrite(rs,rt,rt,bitset<32> (0),0);
      addr_result = imm.to_ulong() + myRF.ReadData1.to_ulong();
      addr = bitset<32> (addr_result);
      myDataMem.MemoryAccess(addr,myRF.ReadData2,0,1);
    
    }
    //j
    else if(fetch.to_string().substr(0,6) == "000010"){
      //get lst 26 bits(address)
      bitset<32> mask = 0b000011111111111111111111111111;
      bitset<26> jump_address = (fetch & mask).to_ulong();

      //j address // PC ← {PC+4[31:28], address, 00}
      pc = (0b00|(pc) >> 28) | (jump_address.to_ulong() );
      
    
    }

    /**** You don't need to modify the following lines. ****/
    count++;
    myRF.OutputRF(); // dump RF;    
  }
  myDataMem.OutputDataMem(); // dump data mem

  return 0;
}



