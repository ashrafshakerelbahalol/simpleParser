#include <iostream>
#include<fstream>
#include<ctype.h>
#include<string>
using namespace std;

enum token{
	int_symbol, float_symbol , cout_symbol,  cout_op_sy, lparn_sy , rparn_sy , Addop , multop , id , num , error_sy , end_source_sy , equal_symbol
};

class parser{

	private:
		ifstream f; // s =1
		token curr_token;

		token check_reserved (string s){
			if (s=="int") return int_symbol;
            else if (s=="float") return float_symbol;
			else if (s=="cout") return cout_symbol;
			else return id;
		}

		token get_token(){

			char ch;
			string s;
			int state = 0;


			while(state>=0 && state <=11 && !f.eof()){

				switch(state){
					case 0:
						f.get(ch);//int
						if (f.eof()) return end_source_sy;
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
						else state = 11;
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
						if(ch=='<') return cout_op_sy;
						else return error_sy;
						break;



					case 6:
						return Addop; break;

					case 7:
						return multop; break;

					case 8:
						return lparn_sy; break;

					case 9:
						return rparn_sy; break;
                    case 10:
                        return equal_symbol; break;
					case 11:
						return error_sy; break;


				}
			}
			return end_source_sy;
		}


	string name(token t){

		switch(t){


			case cout_symbol: return "cout token";break;
			case cout_op_sy: return "<< token";break;
			case id: return "identifier";break;
			case int_symbol: return "int token";break;
            case float_symbol: return "float token";break;
			case num: return "num token";break;
			case Addop: return "+ or - token";break;
			case multop : return "* or / token";break;
			case lparn_sy: return "( token";break;
			case rparn_sy: return ") token";break;
			case error_sy: return "error token";break;
            case equal_symbol: return "equal token";break;
			case end_source_sy: return "End of file";break;

		}

		return "error token";
	}


	public:


		parser(string filename){
			f.open(filename.c_str());
		}

		~parser(){
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
			program();
			match(end_source_sy);

		}

		//<program> ::= int  id  = <expr-seq> |  cout  << <expr-seq> | int  id  = <expr-seq>

		void program(){
			if(curr_token == int_symbol){ //1
				match(int_symbol);
                match(id);
				match(equal_symbol);
				expr_seq();

                program();
			}
            else if(curr_token == float_symbol){ //2
                match(float_symbol);
                match(id);
                match(equal_symbol);
                expr_seq();
                program();
            }
			else if(curr_token == cout_symbol){ //3
				match(cout_symbol);
				match(cout_op_sy);
				expr_seq();
                program();
			}
            else if(curr_token == end_source_sy){
                match(end_source_sy);
            }
			else
				syntax_error(curr_token);
		}

		//<expr-seq>::= <expr> { <expr> }

		void expr_seq(){
			expr();
			while(curr_token == id || curr_token == num ||  curr_token == lparn_sy || curr_token == rparn_sy ||  curr_token == Addop || curr_token == multop  ){
				expr();
			}
		}

		//<expr>    ::= <id>|<int> | <real> |  ( + <expr-seq>) | ( * <expr-seq>)

		void expr(){

			switch(curr_token){
				case id: match(id); break;
				case num : match(num);break;
				case lparn_sy:
					match (lparn_sy);
					if (curr_token == num){  
                        match(num);
						if (curr_token==Addop){
                        match (Addop);
						expr_seq();
						match(rparn_sy);
                        }
                        else if (curr_token == multop ){
                            match (multop);
                            expr_seq();
                            match(rparn_sy);
                        }
					}
                    else{
						syntax_error(curr_token);
					}

					break;


			}


		}


};


int main()
{
    string name ="input.txt";
    parser p(name);
    p.sample_parser();
    p.~parser();
}