#include "request_parser.h"


BodyParser::BodyParser(std::string request){
    get_body(request);
}

std::string BodyParser::get_body(std::string request){

    std::string content_length = "";
    int length;

    size_t position = request.find("Content-Length: ");//16 caracteres
    int espaco[2], qnt_espacos = 0;
    for(int i = position; i<request.length(); i++){
        if(request[i] == ' ' and qnt_espacos == 0){
            qnt_espacos++;
            espaco[0] = i;
        }
        else if(request[i] == ' ' and qnt_espacos == 1){
            qnt_espacos++;
            espaco[1] = i;
        }
    }

    for(int i = espaco[0]; i<espaco[1]+1; i++){
        content_length+=request[i];
    }

    length = stoi(content_length);

    for(int i = request.length()-length; i<request.length(); i++){
        body+=request[i];
    }


    return body;
    

}

std::string BodyParser::get_value(std::string name){
    int position = body.find(name+"=");

    std::string value = "";

    for(int i = position; i<body.length(); i++){
        if(body[i] != '&' and body[i] != ' ' ){
            value+=body[i];
        }
        else{
            i = body.length();
        }
    }

    return value;
}

std::string BodyParser::get_multipart_fd_filename(std::string request){


    int aspas=0;
    std::string filename="";
    int pos = request.find("filename=");

    for(int i = pos; i<request.length() and aspas<2; i++){
        if(request[i] == '\"')
            aspas++;
        else if(aspas == 1){
            filename+=request[i];    
        }
    }

    return filename;
}


std::string BodyParser::get_multipart_fd_value(std::string request){

    //request must be a string with newlines,something like
    // <html>
    //      <body>
    //          <p>hi</p>
    // ... and so on

    request = get_multipart_body(request);

    int newlines = 0;
    std::cout<<request<<" REQUEST DEBUG\n";
    for(int i = 0; i<request.length(); i++){
        if(request[i] == '\n'){
            newlines++;
        }
    }

    int _newlines=0;
    std::string return_value="";
    for(int i = 0; i<request.length() and _newlines<newlines-1; i++){
        if(request[i] == '\n' and _newlines<3)
            _newlines++;
        else if(request[i] == '\n' and _newlines>=3){
            _newlines++;
            return_value+='\n';
        }
        else if(_newlines >= 3 and _newlines<newlines-1){
            return_value+=request[i];
        }
    }

    return return_value;
}

std::string BodyParser::get_multipart_body(std::string request){

    std::string content_length = "";
    int length;
    std::string multipart_body="";

    size_t position = request.find("boundary=");//9 characters
    std::string boundary = "--";
    for(int i = position+9; i<request.length(); i++){
        if(request[i] != '\n')
            boundary+=request[i];
        else
            break;
    }


    size_t begining, ending;
    begining = request.find(boundary);

    bool start = false;
    for(int i = begining+boundary.length(); i<request.length()-boundary.length()-3; i++){
        
        if(request[i] == '<' and start == false) //every html file starts with <
            start = true;

        if(start)
            multipart_body += request[i];

        // std::cout<<"in Loop: "<<i<<"\n";
    }

    return multipart_body;

    //fuck this code, that shit is so bad

}
