#include "read_parameters.h"
#define COMMENT_CHAR '-'

// judge if is a comment char
bool IsCommentChar(char c)
{
    switch(c) {
    case COMMENT_CHAR:
        return true;
    default:
        return false;
    }
}

// delete internal spaces 
void deleteSpace(string& str){
    char * tail = &str[0];
    char * next = &str[0];

    while(*next){ 
        if(!isspace(*next)){ 
            *tail = *next;
            tail++;
        }
        next++;
    }
    *tail='\0';
}

// getting rid of spaces
void TrimSpaces(string& str)
{    
    int i, start_pos, end_pos;

    if (str.empty()) {
        return;
    }
    
    // set start position for the string
    for (i = 0; i < str.size(); ++i) {
        if (!isspace(str[i])) {
            break;
        }
    }

    // the string is a blank one
    if (i == str.size()) { 
        str = "";
        return;
    }
    
    start_pos = i;
    
    // find the last position for the string
    for (i = str.size() - 1; i >= 0; --i) {
        if (!isspace(str[i])) {
            break;
        }
    }
    end_pos = i;
    
    str = str.substr(start_pos, end_pos - start_pos + 1);
    deleteSpace(str);
}

// getting rid of spaces
void TrimMarks(string& str)
{    
    int i, start_pos, end_pos;

    if (str.empty()) {
        return;
    }
    
    // set start position for the string
    for (i = 0; i < str.size(); ++i) {
        if (isdigit(str[i])) {
            break;
        }
    }

    // the string is a blank one
    if (i == str.size()) { 
        str = "";
        return;
    }
    
    start_pos = i;
    
    // find the last position for the string
    for (i = str.size() - 1; i >= 0; --i) {
        if (isdigit(str[i])) {
            break;
        }
    }
    end_pos = i;
    
    str = str.substr(start_pos, end_pos - start_pos + 1);
}

// delete internal [] and space, saparate by ,
vector<string> extractArray(string& str){
    TrimMarks(str);
    vector<string> arr;
    char * tail = &str[0];
    char * next = &str[0];

    while(*next){ 
        if(isdigit(*next)|| *next == ',' || *next == '.'){ 
            *tail = *next;
            tail++;
        }
        next++;
    }
    *tail='\0';

    if (str.find(',') == -1)
    {
		arr.push_back(str);
	}
    else
    {
    char *token = strtok(&str[0], ", ");
    while (token != NULL) {
        arr.push_back(token);
        token = strtok(NULL, ", ");
    }
    }

    return arr;
}


string read_parameters::getValue(const string & name)
{   
    infile->seekg(0, ios::beg);
	string line;
	string new_line;
    int start_pos = 0;
    int end_pos, pos;
    getline(*infile, line);
    while(true)
    {  
		if (line.empty() || line.find(COMMENT_CHAR) != -1 || line.find('=') == -1)
		{
            if (!infile->eof())
               getline(*infile, line);
            else break;
		}

        pos = line.find('=');
        end_pos = line.size() - 1;

		string na = line.substr(0, pos);
		TrimSpaces(na);
		if(na==name)
		{   

			string value=line.substr(pos + 1, end_pos + 1- (pos + 1));
			TrimSpaces(value);
			return  value;
		}
        else
        {   
            if (!infile->eof())
               getline(*infile, line);
            else break;
        }
    
    }

    // return "" indicating not find the value for the parameter
	return "";
}

void read_parameters::set_splashes()
{      
     string rs = getValue("rs");
     string xs = getValue("xs");
     string ys = getValue("ys");

     if (rs == "") 
     {
         cerr << "Not find value for radius of splashes!! " << endl;
         exit(EXIT_FAILURE); 
     }
     else if (xs == "") 
     {  
        cerr << "Not find value for x-coordinates of splashes!! " << endl;
        exit(EXIT_FAILURE); 
     }
     else if (ys == "") 
     {
        cerr << "Not find value for y-coordinates of splashes!! " << endl;
        exit(EXIT_FAILURE); 
     }
     else
     {    
          vector<string> rs_vec = extractArray(rs);
          vector<string> xs_vec = extractArray(xs);
          vector<string> ys_vec = extractArray(ys);

          int rs_size = rs_vec.size();
          int xs_size = xs_vec.size();
          int ys_size = ys_vec.size();

          if (rs_size == xs_size && xs_size == ys_size)
             {    
                  num_splash = rs_size;
                  read_parameters::rs = new double[num_splash];
                  read_parameters::xs = new double[num_splash];
                  read_parameters::ys = new double[num_splash];

                  for (int i = 0; i < num_splash; ++i)
                  {
                      read_parameters::rs[i] = atof(rs_vec[i].c_str());
                      read_parameters::xs[i] = atof(xs_vec[i].c_str());
                      read_parameters::ys[i] = atof(ys_vec[i].c_str());
                      
                      // positive checking 
                      if (read_parameters::xs[i] < 0 ||  read_parameters::ys[i] < 0)
                        {
                            cerr << "position of xs and ys of the splash should be positive !!!" << endl;
                            exit(EXIT_FAILURE); 
                        }
                      // range checking
                      if (read_parameters::xs[i] > length || read_parameters::ys[i] > width)
                        {
                            cerr << "position of xs or ys of splashes out of range !!!" << endl; 
                            exit(EXIT_FAILURE); 
                        }                      
                  }

             }

          else 
          {
            cerr << "The radius, x-coordinates and y-coordinates for the splashes are not the same size!!!" << endl;
            exit(EXIT_FAILURE); 
          }
     }


}

void read_parameters::print_parameters()
{    
     cout << "Printing parameters: " << endl << "------------------------------------------------------------" << endl;
     cout << "length: " << length << endl;
     cout << "width: " << width << endl;
     cout << "imax: " << imax << endl;
     cout << "jmax: " << jmax << endl;
     cout << "c: " << c << endl;
     cout << "dt: " << dt << endl;
     cout << "t_end: " << t_end << endl;
     cout << "t_out: " << t_out << endl;
     cout << "boundary_type: " << boundary_type << endl;
     cout << endl << "Initial splashes: " << endl << "------------------------------------------------------------" << endl;
     
     for (int i = 0; i < num_splash; ++i)
     {
          cout << "splash located at (" << xs[i] << ", " << ys[i] <<") with radius " << rs[i] <<". " << endl;
     }
}


read_parameters::read_parameters()
{
	infile=new ifstream("parameters.ini", ios_base::in);
    if (!infile) 
	{
        cout << "can't open config file 'parameters.ini'! " << endl;
        exit(EXIT_FAILURE);
    }
    
    if(getValue("length") == "")
    {   
       cerr << "invalid input!! please input 'length' as a double." << endl;
       exit(EXIT_FAILURE); 
    } 
    else length = stof(getValue("length"));

    if(getValue("width") == "")
    {  
       cerr << "invalid input!! please input 'width' as a double." << endl;
       exit(EXIT_FAILURE); 
    } 
    else width = stof(getValue("width"));
    
    if(getValue("imax") == "")
    {   
       cerr << "invalid input!! please input 'imax' as a int." << endl;
       exit(EXIT_FAILURE); 
    }
    else imax = stoi(getValue("imax"));

    if(getValue("jmax") == "")
    {   
       cerr << "invalid input!! please input 'jmax' as a int." << endl;    
       exit(EXIT_FAILURE); 
    } 
    else jmax = stoi(getValue("jmax"));

    if(getValue("c") == "")
    {
       cerr << "invalid input!! please input 'c' as a double." << endl;
       exit(EXIT_FAILURE); 
    }     
    else c = stof(getValue("c"));

    if(getValue("dt") == "")
    {   
        cerr << "invalid input!! please input 'dt' as a double." << endl;
        exit(EXIT_FAILURE); 
    }     
    else dt = stof(getValue("dt"));

    if(getValue("t_end") == "")
    {
       cerr << "invalid input!! please input 't_end' as a double." << endl;   
        exit(EXIT_FAILURE); 
    }    
    else t_end = stof(getValue("t_end"));


    if(getValue("t_out") == "")
    {   
       cerr << "invalid input!! please input 't_out' as a double." << endl;  
       exit(EXIT_FAILURE); 
    }      
    else t_out = stof(getValue("t_out"));

    if(getValue("boundary_type") != "periodic" && getValue("boundary_type") != "Dirichlet" && getValue("boundary_type") != "Neumann")
    {
       cerr << "invalid input!! please input 'boundary_type' in one of the type: 'periodic', 'Dirichlet' or 'Neumann'." << endl; 
       exit(EXIT_FAILURE); 
    }     
    else boundary_type = getValue("boundary_type");

    if(getValue("display_option") != "true" && getValue("display_option") != "false")
    {
       cerr << "invalid input!! please input 'display_option' with 'true' or 'false'." << endl; 
       exit(EXIT_FAILURE); 
    }     
    else 
    {
        if (getValue("display_option") == "true")
        {
            display_option = true;
        }
        else if(getValue("display_option") == "false")
        {
            display_option = false;
        }
    }

    if(getValue("output") != "true" && getValue("output") != "false")
    {
       cerr << "invalid input!! please input 'output' with 'true' or 'false'." << endl; 
       exit(EXIT_FAILURE); 
    }     
    else 
    {
        if (getValue("output") == "true")
        {
            output = true;
        }
        else if(getValue("output") == "false")
        {
            output = false;
        }
    }

    set_splashes();

}
 
read_parameters::~read_parameters()
{
    infile->close();
}