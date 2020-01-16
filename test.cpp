#include<fstream>
using namespace std;

typedef int8_t t_data;
const int tot_bits = sizeof(t_data)*8;


/* returns pairity of mem_block */
t_data get_pairity(t_data m) { 
    //t_data lsb = m&1;
    t_data count = 0; 

    while (m) { 
        m &= (m- 1); 
        count++; 
    } 

    return (count&1); 
} 

/* shifts new block and places prev lsb as msb */
t_data get_trasmit_block(t_data &m, t_data lsb){
    t_data msb = lsb << (tot_bits-1);       //make old lsb the new msb
    lsb = m&1;                              //backup current m's lsb
    m >>= 1;                                //make space for new msb
    m = m | msb;                            //set the new msb
    return m;
}


int main(int argc, char **argv){
    t_data mem;
    bool first_mem_block = 1;

    if(argc == 1){
        printf("Test file not specified.\n");
        exit(1);
    }//exit if input file is not sprcified.


    /*file from which data stream is read*/
    ifstream fin(argv[1], ios::in | ios::binary);

    /*file to which paired binaries are witten*/
    ofstream fout("output.bin",ios::out | ios::binary);

    /*log file for output verification*/
    ofstream log_f("log.txt",ios::out);

    t_data lsb;
    while(fin.read((char*)&mem,sizeof(t_data))){
        
        if(first_mem_block){
            lsb = mem & 1;//backup lsb
            if(get_pairity(mem)==1)mem ^= 1;
            first_mem_block = 0;
        }
        else{
            get_trasmit_block(mem,lsb);                 
            if(get_pairity(mem)==1)
            mem ^= 1;
        }

        fout.write((char *) &mem, sizeof(t_data));//write to output binary file
        log_f << mem << endl;                     //write to log file.
    }//loop until whole file is read.


    fin.close();
    fout.close();
    log_f.close();
    return 0; 
}
