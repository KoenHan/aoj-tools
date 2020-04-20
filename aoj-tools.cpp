// コンパイルからテストまでやってくれます
// input*.txtやoutput*.txtの書き忘れに注意
// このファイルをコンパイルするときは"g++ aoj-tools.cpp -o aoj-tools"でやることをお勧めします

#include <bits/stdc++.h>
#include <unistd.h>
#include <chrono>
using namespace std;

inline bool check_fsize_is_zero(const string fname, const string message){
    FILE *fp = fopen(fname.c_str(), "r");
    int c = getc(fp);
    bool flag = false;
    if(c == EOF) {
        if(message.size()) puts(message.c_str());
        flag = true;
    }
    fclose(fp);
    return flag;
}

inline bool check_file_ex(const string fname, const string message){
    FILE *fp;
    bool flag = true;
    if(( fp = fopen(fname.c_str(), "r")) == NULL) {
        if(message.size()) puts(message.c_str());
        flag = false;
    }
    fclose(fp);
    return flag;
}

inline void print_all(const string input, const string output, const string exresult){
    const string com4 = "cat " + input;
    const string com5 = "cat " + output;
    const string com6 = "cat " + exresult;
    const string eof = "\x1b[34m[EOF]\x1b[0m";

    puts("\x1b[35m[ Input ]\x1b[0m");
    system(com4.c_str());
    cout << eof << endl;
    puts("\x1b[35m[ Expected ]\x1b[0m");
    system(com5.c_str());
    cout << eof << endl;
    puts("\x1b[35m[ Received ]\x1b[0m");
    system(com6.c_str());
    cout << eof << endl << endl;
    return;
}

inline void test(){
    const string wa = "\x1b[31mWrong Answer\x1b[0m";
    const string main = "main.cpp";
    const string exresult = ".exresult.txt";
    const string com1 = "g++ " + main;
    const string com3 = "rm " + exresult;
    const string num[] = {"1", "2", "3", "4", "5"};
    string com2;
    string input, output;
    // char dir[LEN]
    // getcwd(dir, LEN);

    if(!check_file_ex(main, "--- Can not find \"main.cpp\". ---"))
        exit(EXIT_FAILURE);

    if(!system(com1.c_str())){
        puts("--- Compile success. --- ");

        FILE *fpoutput, *fpexresult;
        int co, ce, pre_co, pre_ce = -1;

        for(string n : num){
            input = "input" + n + ".txt";
            if(!check_file_ex(input, "")) continue;
            else  {
                cout << "# " << input << " ... ";
                if(check_fsize_is_zero(input, "\x1b[31m"+input+" is empty.\x1b[0m")) continue;
            }

            output = "output" + n + ".txt";
            if(check_file_ex(output, "\x1b[31m"+output+" does not exist.\x1b[0m"))
                fpoutput = fopen(output.c_str(), "r");
            else continue;

            com2 = "./a.out <" + input + "> " + exresult;
            system(com2.c_str());

            if(check_file_ex(exresult, "\x1b[31mCan not open "+exresult+".\x1b[0m"))
                fpexresult = fopen(exresult.c_str(), "r");
            else exit(EXIT_FAILURE);

            bool flag = true;
            while (true) {
                co = fgetc(fpoutput); ce = fgetc(fpexresult);
                if(co == EOF || ce == EOF) break;
                else if(co != ce) {
                    cout << wa << endl;
                    print_all(input, output, exresult);
                    flag = false;
                    break;
                }
                pre_co = co; pre_ce = ce;
            }
            fclose(fpoutput); fclose(fpexresult);
            if(flag) {
                if(co == EOF && ce == EOF) {
                    cout << "\x1b[32mAccepted\x1b[0m" << endl;
                } else if(co == EOF && pre_co != '\n') {
                    cout << "\x1b[31m" << output << " format error\x1b[0m" << endl;
                    print_all(input, output, exresult);
                } else if(ce == EOF && pre_ce == -1) {
                    puts("\x1b[31mNothing Received.\x1b[0m\n");
                    // break;
                } else {
                    puts("\x1b[31mPresentation Error\x1b[0m");
                    print_all(input, output, exresult);
                }
            }
        }
        if(check_file_ex(exresult, "")) system(com3.c_str());
    } else puts("--- Compile failed. ---");

    return;
}

inline void gen(const char* dname){
    const string ct = "\"#include <bits/stdc++.h>\n"
                        "#define rep(i, n) for(int i = 0; i < (n); i++)\n"
                        "using namespace std;\n"
                        "using ll = long long;\n\n"
                        "int main(){\n\n"
                        "\treturn 0;\n"
                        "}\"";
    const string dn = dname;
    const string com1 = "mkdir "+dn+" && echo "+ct+" > "+dn+\
                        "/main.cpp && touch "+dn+\
                        "/input1.txt && touch "+dn+\
                        "/output1.txt";
    if(!system(com1.c_str())) puts("--- Creation success. ---");
    else puts("--- Creation failed. ---");

    return;
}

int main(int argc, char* argv[]){
    if(argc == 2 && !strcmp(argv[1], "test")) test();
    else if(argc == 3 && !strcmp(argv[1], "gen")) gen(argv[2]);
    else {
        const string help = "Usage:\n"
                            "aoj-tools test -- to test \"main.cpp\" in your workspace.\n"
                            "aoj-tools gen <dir name> -- to generate workspace whose name is <dir name>.";
        puts(help.c_str());
    }
    return 0;
}