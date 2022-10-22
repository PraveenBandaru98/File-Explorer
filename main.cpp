#include<iostream>
#include<bits/stdc++.h>

#include <unistd.h>
#include<dirent.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<stdlib.h>
#include<time.h>
#include <pwd.h>
#include <fcntl.h>
#include <grp.h>
#include<sys/ioctl.h>
using namespace std;
#include <termios.h>
#include <fstream>
void setpos(int);
string home=getpwuid(getuid())->pw_dir;    // to store home directory
string app;
stack<string> lst,rst;
vector<vector<string>> vecall;
vector<struct stat> vecstat;

int window_size=10;

struct termios newi = {0};


void startnormalmode(string);
void clearscreen();                 // sort files based on name
bool mysort( const vector<string>& v1,                          
               const vector<string>& v2 ) {
 return v1[0] < v2[0];
}


string path(string inp){                        // to deal with relative and absolute paths
    if(inp[0]=='.' and inp.size()==1){
        char tmp[256];              // start with .
            getcwd(tmp, 256);
    
            string tpath(tmp);

            return tpath;
    }
    else if(inp[0]=='.' and inp[1]=='/')   // start with ./
    {

            char tmp[256];
            getcwd(tmp, 256);
    
            string tpath(tmp);

            int te=inp.size();
            
            return tpath+'/'+inp.substr(2,te-2);
    }
    else if(inp[0]=='~'){    // start with tilta
        int te=inp.size(); 
        return home+inp.substr(1,te-1);
    }
    else if(inp[0]=='/'){    // start with slash

        char tmp[256];
            getcwd(tmp, 256);
    
            string tpath(tmp);

            int te=inp.size();
            
            return tpath+inp;

    }
    else{    //start with character
        char tmp[256];
            getcwd(tmp, 256);
    
            string tpath(tmp);
            return tpath+'/'+inp;
    }

    

}
void search(string path,int &f,string fn){       // search command-->recursive
    
                    if(f==1){
                        return;
                    }

                DIR *dir;
                struct dirent *ent;
        if ((dir = opendir (path.c_str())) != NULL) {
        while ((ent = readdir (dir)) != NULL) {
                string zz="";
                zz=zz+'.';
                
                if(string(ent->d_name)==zz or string(ent->d_name)==".."){
                    //cout<<string(ent->d_name);

                    continue;
                }

                 if(ent->d_name==fn){
                        f=1;
                        closedir (dir);
                        return;
                    }
                
                 int ford=ent->d_type;
                 if(ford==DT_DIR){
                    string p=path+"/"+ent->d_name;
                    search(p,f,fn);
                    if(f==1){
                        closedir (dir);
                        return;
                    }

                }
                


                 
             
        }
        closedir (dir);
        } 
        


}
void dirdelete(string path){       // delete directory recursive


            DIR *dir;
                struct dirent *ent;
        if ((dir = opendir (path.c_str())) != NULL) {
        while ((ent = readdir (dir)) != NULL) {
                string zz="";
                zz=zz+'.';
                
                if(string(ent->d_name)==zz or string(ent->d_name)==".."){
                    //cout<<string(ent->d_name);

                    continue;
                }

                
                
                 int ford=ent->d_type;
                 if(ford==DT_DIR){
                    string p=path+"/"+ent->d_name;
                    dirdelete(p);
                    rmdir(p.c_str());

                }
                else{
                    unlink((path+"/"+ent->d_name).c_str());
                }
                


                 
             
        }
        closedir (dir);
        } 












}


void copyfile(string s1,string s2)          // copy two files
{

char b;
	int fin,fout, nread;
	fin = open(s1.c_str(),O_RDONLY);
	fout = open(s2.c_str(),O_WRONLY | O_APPEND);
	while((nread = read(fin,&b,1))>0){
		write(fout,&b,nread);
    }
}
int copybef(string,string);
int copydir(string a, string b){           // copy directory
    

                DIR *dir;
                struct dirent *ent;
        if ((dir = opendir (a.c_str())) != NULL) {
        while ((ent = readdir (dir)) != NULL) {
                string zz="";
                zz=zz+'.';
                
                if(string(ent->d_name)==zz or string(ent->d_name)==".."){
                    //cout<<string(ent->d_name);

                    continue;
                }

                
                 int ford=ent->d_type;
                 if(ford==DT_DIR){
                    



                string t1=a+'/'+ent->d_name;
                string t2=b+'/'+ent->d_name;
                struct stat s;
                stat(t1.c_str(), &s);
                mode_t pe=s.st_mode;
                    int status;
                status = mkdir(t2.c_str(),pe);
                if(status==-1){
                     closedir(dir);
                    return 0;

                }
                

                int zx=copydir(t1,t2);
                if(zx==0){

                    closedir(dir);
                    return 0;

                }







                }
                else{




                    string t1=a+'/'+ent->d_name;
                    string t2=b+'/'+ent->d_name;

                    int stn=copybef(t1,t2);
                    if(stn==0){

                    closedir(dir);
                    return 0;

                    }


                }
                


                 
             
        }
        closedir (dir);
        }
        else{


            return 0;


        } 
 return 1;       


}
int copybef(string a , string b){                                           


            struct stat st;
             stat(a.c_str(), &st);
             
             int sta=creat(b.c_str(),00777);
             
             
             if(sta==-1){
                 cout<<"unsuccessfull";
                 return 0;
             }

             
            
                
            
             copyfile(a,b);
             mode_t pe=st.st_mode;
             chmod(b.c_str(),pe);
             return 1;
}


void check(string st){                              // to check the command given in command mode
    vector<string> given;
    string temp="";
    for(int i=0;i<st.size();i++){
        if(st[i]==' '){
            given.push_back(temp);
            temp="";
        }
        else{
            temp+=st[i];
        }
    }
    given.push_back(temp);
    if(given.size()==0){
        return;
    }
    int len=given.size();
    string dest=path(given[len-1]);
    
    
    
    
    if("copy"==given[0]){


        
        


        
        DIR *dir;
        struct dirent *ent;
        int zx;
        int f;
        for(int i=1;i<len-1;i++){
            f=0;
            char tmp[256];
            getcwd(tmp, 256);
            

            string tp(tmp);
        




            if ((dir = opendir (tp.c_str())) != NULL) {
        while ((ent = readdir (dir)) != NULL) {
                

                 if(ent->d_name==given[i]){
                     f=1;
                   
            int ford=ent->d_type;
            
            if(ford==DT_DIR){


                string a=tp+'/'+given[i];
                string b=dest+'/'+given[i];
                struct stat s;
                stat(a.c_str(), &s);
                mode_t pe=s.st_mode;
                    int status;
                status = mkdir((dest+'/'+given[i]).c_str(),pe);
                if(status==-1){
                    cout<<"unsuccessfull";
                     closedir(dir);
                    return;

                }
                

                zx=copydir(a,b);
                if(zx==0){

                    cout<<"unsuccessfull";
                    closedir(dir);
                    return;

                }

            }
            else{
                string a=tp+'/'+given[i];
                string b=dest+'/'+given[i];
                
                zx=copybef(a,b);
                if(zx==0){
                    cout<<"unsuccessfull";
                    closedir(dir);
                    return;

                }
            }

                 }
            



            

        }
        if(f==0){
            cout<<"unsuccessfull";
            return;
        }
    closedir(dir);
        }
        else{
            cout<<"unsuccessfull";
            return;
        }
        
        
        }
        cout<<"successfull";
    }


     
    else if(given[0]=="move"){




        DIR *dir;
        struct dirent *ent;
        int zx;
        int f;
        for(int i=1;i<len-1;i++){
            f=0;
            char tmp[256];
            getcwd(tmp, 256);
            

            string tp(tmp);
        




            if ((dir = opendir (tp.c_str())) != NULL) {
        while ((ent = readdir (dir)) != NULL) {
                

                 if(ent->d_name==given[i]){
                     f=1;
                   
            int ford=ent->d_type;
            
            if(ford==DT_DIR){


                string a=tp+'/'+given[i];
                string b=dest+'/'+given[i];
                struct stat s;
                stat(a.c_str(), &s);
                mode_t pe=s.st_mode;
                    int status;
                status = mkdir((dest+'/'+given[i]).c_str(),pe);
                if(status==-1){
                    cout<<"unsuccessfull";
                     closedir(dir);
                    return;

                }
                

                zx=copydir(a,b);
                if(zx==0){

                    cout<<"unsuccessfull";
                    closedir(dir);
                    return;

                }
                dirdelete(a);
                 int sta=rmdir(a.c_str());
                 if(sta==-1){
                     cout<<"unsuccessful..";
                     closedir (dir);
                     return;

                 }



            }
            else{
                string a=tp+'/'+given[i];
                string b=dest+'/'+given[i];
                
                zx=copybef(a,b);
                if(zx==0){
                    cout<<"unsuccessful";
                    closedir(dir);
                    return;

                }
                int status=unlink(a.c_str());
                if(status==-1){
                    cout<<"unsuccessful";
                    closedir(dir);
                    return;
                }
                

            }

                 }
            



            

        }
        if(f==0){
            cout<<"unsuccessfull";
            return;
        }
    closedir(dir);
        }
        else{
            cout<<"unsuccessfull";
            return;
        }
        
        
        }
        cout<<"successfull";
    }


        


        


    
    else if(given[0]=="rename"){


        	if (rename(given[1].c_str(),given[2].c_str()) == 0){
                    cout<<"successfull";
            }
                
            else{
                cout << "unsuccessfull";
            }

        
    }
    else if(given[0]=="create_file"){


            int status;
            mode_t mode = S_IXUSR | S_IXGRP | S_IRUSR | S_IWUSR  | S_IRGRP | S_IWGRP | S_IROTH;

            
        status = creat((dest+'/'+given[len-2]).c_str(), mode);
        if(status==-1){
            cout<<"unsuccessfull";
        }
        else{
            cout<<"successfull";
        }
        
    }
    else if(given[0]=="create_dir"){


        mode_t mode = S_IXUSR | S_IXGRP | S_IRUSR | S_IWUSR  | S_IRGRP | S_IWGRP | S_IROTH;
        int status;
        status = mkdir((dest+'/'+given[len-2]).c_str(),mode);
        if(status==-1){
            cout<<"unsuccessfull";
        }
        else{
            cout<<"successfull";
        }
          

        
    }
    else if(given[0]=="delete_file"){


        string tpath=path(given[1]);

        int status;
        string file=tpath;
        status=unlink(file.c_str());
        if(status==-1){
            cout<<"unsuccessful";
        }
        else{
            cout<<"successful";
        }

        
    }
    else if(given[0]=="delete_dir"){

        /*char tmp[256];
        getcwd(tmp, 256);*/
    
        string tpath=path(given[1]);
        temp=tpath;
        int i;
            for(i=(temp.size())-1;i>=0;i--){
                if(temp[i]=='/'){
                    break;
                }
            }
            if(i<=0){cout<<"unsuccessfull"; return;}  
        tpath=temp.substr(0, i+1);
        string st=temp.substr(i+1,temp.size()-(i+1));

        DIR *dir;
        struct dirent *ent;
        int flag=0;
        if ((dir = opendir (tpath.c_str())) != NULL) {
        while ((ent = readdir (dir)) != NULL) {

             if(ent->d_name==st){
                flag=1;
                 string path=tpath+"/"+ent->d_name;
                 
                 dirdelete(path);
                 int sta=rmdir(path.c_str());
                 if(sta==-1){
                     cout<<"unsuccessfull.";
                     closedir (dir);
                     return;

                 }
             }
        }
        closedir (dir);
        } 
        if(flag==0){
            cout<<"unsuccessfull";
        }
        else{
            cout<<"successfull";
        }


        



        

        
    }
    else if(given[0]=="goto"){

        int status;

        status = chdir (dest.c_str());
        if(status ==-1){
            cout<<"unsuccessfull";
        }
        else{
            cout<<"successfull";
        }


        
    }
    else if(given[0]=="search"){


        

        int f=0;

        char tmp[256];
        getcwd(tmp, 256);
    
        string tpath(tmp);
        
        
        
        search(tpath,f,given[1]);
        if(f==0){
            cout<<"False";
        }
        else{
            cout<<"True";
        }



    }
    else{
        cout<<"Wrong command";
    }

}

void commandmode(){             // command mode
        chdir(app.c_str());
        winsize par;
        ioctl(0,TIOCGWINSZ,&par);
        int row=par.ws_row;
        int col=par.ws_col;
        setpos(row-1);
        printf("\33[2K\r");
        cout<<"COMMAND MODE";
        setpos(row);
        printf("\33[2K\r");
        while(1){
            //printf("\33[2K\r");
        unsigned char ch1;
		unsigned char ch2 ;
		unsigned char ch3 ;
		ch2=0;
		ch3=0;
		ch1=cin.get();
		if(ch1==10){

				//cout<<"enter";
			}
        else if(ch1==27){
            
             return;
            }

    else if(ch1==113){  // quit
            clearscreen();
            newi.c_lflag |= ECHO;
            //keeping icanon flag
            newi.c_lflag |= ICANON;
            //restoring terminal state
            if (tcsetattr(0, TCSADRAIN, &newi) < 0)
            {   // checking for error
            perror ("tcsetattr ~ICANON");
            }
            exit(0);
            }
            else{
                printf("\33[2K\r");
                
                string s="";
                s+=ch1;
                cout<<s;
                while(1){
                    
                    char tem;
                    tem=cin.get();
                    if(s.size()==0 and tem==113){
                                    clearscreen();
                        newi.c_lflag |= ECHO;
                        //keeping icanon flag
                        newi.c_lflag |= ICANON;
                        //restoring terminal state
                        if (tcsetattr(0, TCSADRAIN, &newi) < 0)
                        {   // checking for error
                        perror ("tcsetattr ~ICANON");
                        }
                        exit(0);
                        
                    }
                    else if(s.size()==0 and tem==27){
                        return ;
                    }
                    else if(tem==10){
                        printf("\33[2K\r");
                        check(s);
                        //cout<<".";
                        s="";
                        break;
                    }
                    
                    else if(tem==127){
                        printf("\33[2K\r");
                        s= s.substr(0, s.size()-1);
                        cout<<s;

                    }
                    else{
                        s+=tem;
                        printf("\33[2K\r");
                        cout<<s;
                        
                    }




                }
                

            }
	

        }




}


void clearscreen(){                         // clear screen
    printf("\033[H\033[J");
    
}
void setpos(int c)                      // to set cursor position
{
    int r=0;
    printf("\033[%d;%df",c,r);
    
}
void printdetail(int i){                // print files

    
        vector<string> tem=vecall[i];
        string na=tem[0];
        if(na.size()>15){
            na=na.substr(0,13);
            na=na+"..";
        }
        printf("%-10s\t%-10s\t%-10s\t%-10s\t%-10s\t%-10s\n",na.c_str(),tem[1].c_str(),tem[2].c_str(),tem[3].c_str(),tem[4].c_str(),tem[5].c_str());
        
       


}
void getfiles(string chang){            // to get files from vector
    
    vecstat.clear();
    vecall.clear();
    //char tmp[256];
    //getcwd(tmp, 256);
    char tmp[chang.length()];
    for (int i = 0; i < sizeof(chang); i++) {
        tmp[i] = chang[i];
    }
    string pa=chang;
    DIR *dir;
     //..
    struct dirent *ent;
    
    // check for null
    if ((dir = opendir (tmp)) != NULL) {
        
        //read each dir
        while ((ent = readdir (dir)) != NULL) {
            //chdir(tmp);
            int ford=ent->d_type;
        
        string path=pa+"/"+ent->d_name;
         char p[path.length()];
            int i;
            for (i = 0; i < sizeof(p); i++) {
                p[i] = path[i];
            }
        //stat structure
        struct stat s;
        // function calling
        stat(ent->d_name,&s);
        
        vecstat.push_back(s);
        vector<string> tem;
        tem.push_back(ent->d_name);                         
        tem.push_back(to_string(s.st_size)+" Bytes");

        // get struct of user
        struct passwd *pw = getpwuid(s.st_uid);
        // get struct of group
        struct group  *gr = getgrgid(s.st_gid);

        if(pw != 0){
            // storing name if name is presnt
            tem.push_back(pw->pw_name);
            }
        else{
            // storing user  id if no name
            tem.push_back(to_string(s.st_uid));
        }

        if(gr != 0){
            // storing name if name is presnt
            tem.push_back(gr->gr_name);
        }
        else{
            // storing id if no name
            tem.push_back(to_string(s.st_gid));
        }


        string permi="";
        mode_t pe=s.st_mode;
        if(pe&S_IRUSR){
            permi+="r";
        }
        else{
            permi+="-";
        }
        if(pe&S_IWUSR){
            permi+="w";
        }
        else{
            permi+="-";
        }
        if(pe&S_IXUSR){
            permi+="x";
        }
        else{
            permi+="-";
        }
        
        if(pe&S_IRGRP){
            permi+="r";
        }
        else{
            permi+="-";
        }
        if(pe&S_IWGRP){
            permi+="w";
        }
        else{
            permi+="-";
        }
        if(pe&S_IXGRP){
            permi+="x";
        }
        else{
            permi+="-";
        }


        if(pe&S_IROTH){
            permi+="r";
        }
        else{
            permi+="-";
        }
        if(pe&S_IWOTH){
            permi+="w";
        }
        else{
            permi+="-";
        }
        if(pe&S_IXOTH){
            permi+="x";
        }
        else{
            permi+="-";
        }

        tem.push_back(permi);


            
        

        auto dt=*(gmtime(&s.st_mtime));
string tm=to_string(dt.tm_mday)+"/"+to_string(dt.tm_mon)+"/"+to_string(dt.tm_year+1900)+"  "+to_string(dt.tm_hour)+":"+to_string(dt.tm_min)+":"+to_string(dt.tm_sec);
            

    tem.push_back(tm);


tem.push_back(path);



if(ford==DT_DIR){
	tem.push_back("d");

}
else if(ford==DT_REG){
	tem.push_back("f");
}
else{
	tem.push_back("n");
}

        vecall.push_back(tem);


    
        }
        closedir (dir);
    }
    sort(vecall.begin(),vecall.end(),mysort);


}
void printkfiles(int start,int end)             // to get k files from vector
{
clearscreen();
setpos(1);
    for(int i=start;i<=end;i++)
        {
    printdetail(i);
    }

}

void scrolling(int  list_start,int list_end,int start,int end) // to allow scrolling
{

	
	setpos(1);
	int cursor=1;
	while(1){


		unsigned char ch1;
		unsigned char ch2 ;
		unsigned char ch3 ;
		ch2=0;
		ch3=0;
		ch1=cin.get();
		if(ch1==10){


				//cout<<to_string(ch1)<<" "<<to_string(ch2)<<" "<<to_string(ch3);
			}
		else if (ch1 ==27)
		{
			
			ch2=cin.get();
			ch3=cin.get();
			//cout<<to_string(ch1)<<" "<<to_string(ch2)<<" "<<to_string(ch3);
		}


		if(ch1==27 and ch2==91){
			if(ch3==65){
				if(cursor>1){
					cursor--;
					setpos(cursor);
				}
				
			}
			else if(ch3==66){
				if(cursor<window_size){
					cursor++;
					setpos(cursor);
				}
			}
            else if(ch3==68){//left


                if(lst.size()>1){
                    rst.push(lst.top());
                    lst.pop();
                    startnormalmode(lst.top());

                }
            }

        
            else if(ch3==67)//right arr
            {
                if(rst.size()>=1){      // new added  =1
                    lst.push(rst.top());
                    rst.pop();
                    startnormalmode(lst.top());
                }


            }
		}
        
		else if(ch1==107){    //k
			if(start+cursor-1==start and start>list_start){
				end--;
				start--;
				
				printkfiles(start,end);
				cursor=1;
				setpos(cursor);
			}
		}
		else if(ch1==108){     //l
			if(start+cursor-1==end and end<list_end){
				start++;
				end++;
				printkfiles(start,end);
				cursor=1;
				setpos(cursor);

			}
		}
		else if(ch1==127){     //backspace
            string temp=lst.top();
            int i;
            for(i=(temp.size())-1;i>=0;i--){
                if(temp[i]=='/'){
                    break;
                }
            }
            if(i>0){        //prev dir
            while (rst.size() > 0) {
                
                rst.pop();
            }
            lst.push(temp.substr(0, i+1));
            startnormalmode(lst.top());
            }

        }
		else if(ch1==10){
            if(vecall[start+cursor-1][7]=="d"){
                lst.push(vecall[start+cursor-1][6]);
                                while (rst.size() > 0) {   // new added while loop
                
                                        rst.pop();
                                }


                startnormalmode(lst.top());
            }
            else if(vecall[start+cursor-1][7]=="f"){
                if(!fork()){
                    execlp("vi","vi",vecall[start+cursor-1][6].c_str(),NULL);
                }
                else{
                    //cout<<"failed to open";     newly added
                }
            
                
            }
            else{
                //cout<<"cannot be opened";             newly added
            }
			
		}

        else if(ch1==104){     //home
            if(lst.top()!=home){
            lst.push(home);
            while (rst.size() > 0) {
                
                rst.pop();
            }
            startnormalmode(lst.top());
            }
        }
        else if(ch1==113){  // quit
            clearscreen();
                newi.c_lflag |= ECHO;
    //keeping icanon flag
    newi.c_lflag |= ICANON;
    //restoring terminal state
    if (tcsetattr(0, TCSADRAIN, &newi) < 0)
    {   // checking for error
        perror ("tcsetattr ~ICANON");
    }
            exit(0);
        }
        else if(ch1==58){
            commandmode();
            startnormalmode(lst.top());
            
        }

    }
}




// start normal mode
void startnormalmode(string tempat){                        // to start normal mode

    getfiles(tempat);
    window_size=10;
    window_size=min((int)vecall.size(),window_size);
    int list_start=0;
    int list_end=vecall.size()-1;
    int curr_start=0;
    int curr_end=window_size-1;
    printkfiles(curr_start,curr_end);   //print k files

    scrolling(list_start,list_end,curr_start,curr_end);  // enable scrolling
}












int main(){



        
        //checking for error
        if (tcgetattr(0, &newi) < 0){

                perror("tcsetattr()");
        }
        // turning icanon flag off
        newi.c_lflag &= ~ICANON;
        //turning echo flag off
        newi.c_lflag &= ~ECHO;
        //flags
        newi.c_cc[VMIN] = 1;
        newi.c_cc[VTIME] = 0;
        //changing the terminal state
        if (tcsetattr(0, TCSANOW, &newi) < 0){
                // checking for error
                perror("tcsetattr ICANON");

        }
                


    
    char tmp[256];
    getcwd(tmp, 256);
    
    string tpath(tmp);
    // stroing path of open file (where file stored)
    app=tpath;
    lst.push(tpath);    // pushing to stack
    
    
    startnormalmode(lst.top());



    clearscreen();

    //keeping initial flag
    newi.c_lflag |= ECHO;
    //keeping icanon flag
    newi.c_lflag |= ICANON;
    //restoring terminal state
    if (tcsetattr(0, TCSADRAIN, &newi) < 0)
    {   // checking for error
        perror ("tcsetattr ~ICANON");
    }

    return 1;
}


