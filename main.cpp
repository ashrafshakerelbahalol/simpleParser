#include <iostream>
#include<fstream>
#include<ctype.h>
#include<string>
using namespace std;

enum token{
	int_symbol, float_symbol , cout_symbol,  cout_op_symbol, lparn_symbol , rparn_symbol , Addop , multop , id , num , error_symbol , end_source_symbol , equal_symbol,semicolon_symbol,
    void_symbol,lcurly_bracket,rcurly_bracket
};

class parser{

	private:
		ifstream f; // s =1
		token curr_token;

		token check_reserved (string s){
			if (s=="int") return int_symbol;
            else if (s=="float") return float_symbol;
			else if (s=="cout") return cout_symbol;
            else if (s=="void") return void_symbol;
			else return id;
		}

		token get_token(){

			char ch;
			string s;
			int state = 0;


			while(state>=0 && state <=14 && !f.eof()){

				switch(state){
					case 0:
						f.get(ch);//int
						if (f.eof()) return end_source_symbol;
						else if (isspace(ch)) {s=ch; state = 0;}
						else if (isdigit(ch)) {s=ch; state = 1;}
						else if (ch=='.') {s=ch; state = 2;}
						else if (isalpha(ch)) {s=ch; state = 4;}
						else if (ch=='<') state = 5;
						else if (ch=='+'||ch=='-') state = 6;
						else if (ch=='*'||ch=='/') state = 7;
						else if (ch=='(') state = 8;
						else if (ch==')') state = 9;
                        else if (ch=='=')  state = 10;
                        else if (ch==';')  state = 11;
                        else if (ch=='{')  state = 12;
                        else if (ch=='}')  state = 13;
						else state = 14;
						break;
                     //(1) 12.(3) 3
					case 1:
						f.get(ch);
						if (isdigit(ch)) {s+=ch; state=1;}
						else if (ch=='.') state=3;
						else {f.putback(ch); return num;}
						break;

					case 2:
						f.get(ch);
						if (isdigit(ch)) {s+=ch; state=2;}
						else
						{
							f.putback(ch);
							return num;
						}
						break;

					case 3:
						f.get(ch); //
						if (isdigit(ch)) {s+=ch; state=3;}
						else
						{
							f.putback(ch);
							return num;
						}
						break;

					case 4:
						f.get(ch); //int
						if(isalnum(ch)) {s+=ch; state=4;}
						else
						{
							f.putback(ch);
							return check_reserved(s);
						}
						break;

					case 5:
						f.get(ch);
						if(ch=='<') return cout_op_symbol;
						else return error_symbol;
						break;



					case 6:
						return Addop; break;

					case 7:
						return multop; break;

					case 8:
						return lparn_symbol; break;

					case 9:
						return rparn_symbol; break;
                    case 10:
                        return equal_symbol; break;
                    case 11:
                        return semicolon_symbol; break;
                    case 12:
                        return lcurly_bracket; break;
                    case 13:
                        return rcurly_bracket; break;
					case 14:
						return error_symbol; break;


				}
			}
			return end_source_symbol;
		}


	string name(token t){

		switch(t){


			case cout_symbol: return "cout token";break;
			case cout_op_symbol: return "<< token";break;
            case void_symbol: return "void token";break;
			case id: return "identifier";break;
			case int_symbol: return "int token";break;
            case float_symbol: return "float token";break;
			case num: return "num token";break;
			case Addop: return "+ or - token";break;
			case multop : return "* or / token";break;
			case lparn_symbol: return "( token";break;
			case rparn_symbol: return ") token";break;
			case error_symbol: return "error token";break;
            case equal_symbol: return "equal token";break;
            case semicolon_symbol: return "semicolon token";break;
            case lcurly_bracket: return "left curlybracket token";break;
            case rcurly_bracket: return "right curlybracket token";break;
			case end_source_symbol: return "End of file";break;

		}

		return "error token";
	}


	public:


		parser(string filename){
			f.open(filename.c_str());
		}

		void closefile (){
			f.close();
		}
		void match(token terminal){

			if (curr_token == terminal)
				cout<<name(curr_token)<<" is matched"<<endl;

			else
            {syntax_error(curr_token);}
			curr_token = get_token();
		}

		void syntax_error(token terminal){
			cout<<name(terminal)<<" is not Expected\n";
        }

		void sample_parser(){
			curr_token = get_token();
            match(void_symbol);
            match(id);
            match(lparn_symbol);
            match(rparn_symbol);
            match (lcurly_bracket);
			program();
			match(end_source_symbol);

		}

		//<program> ::= int  id  = <expr-seq> ;|  cout  << <expr-seq> ;| int  id  = <expr-seq>;

		void program(){
			if(curr_token == int_symbol){
                cout<<"******************"<<endl;
                match(int_symbol);
                match(id);
				match(equal_symbol);
				expr_seq();
                match(semicolon_symbol);

                program();
			}
            else if(curr_token == float_symbol){ //2
                cout<<"******************"<<endl;
                match(float_symbol);
                match(id);
                match(equal_symbol);
                expr_seq();
                match(semicolon_symbol);

                program();
            }
			else if(curr_token == cout_symbol){ //3

                cout<<"******************"<<endl;
                match(cout_symbol);
				match(cout_op_symbol);
				expr_seq();
                match(semicolon_symbol);

                program();
			}
            else if(curr_token == rcurly_bracket){
                cout<<"*****************************************************"<<endl;
                match(rcurly_bracket);

            }
			else
				syntax_error(curr_token);
		}

		//<expr-seq>::= <expr> { <expr> }

		void expr_seq(){
			expr();
			while(curr_token == id || curr_token == num ||  curr_token == lparn_symbol || curr_token == rparn_symbol   ){
				expr();
			}
		}

		//<expr>    ::= <id>|<num> |  (  <num> + <num>) |  (  <num> * <num>)

		void expr(){

			switch(curr_token){
				case id: match(id); break;
				case num : match(num);break;
				case lparn_symbol:
					match (lparn_symbol);
					if (curr_token == num){  
                        match(num);
						if (curr_token==Addop){
                        match (Addop);
                            match(num);

                        }
                        else if (curr_token == multop ){
                            match (multop);
                            match(num);

                        }
					}
                    else{
						syntax_error(curr_token);
					}

                case rparn_symbol:
                    match (rparn_symbol);
					break;


			}


		}


};


int main()
{
    string name ="input.txt";
    parser p(name);
    p.sample_parser();
    p.closefile();
}