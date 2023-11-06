#include <iostream>
#include <cstdio>
#include <string>
#include <vector>
#include <sstream>
#include <chrono>
#include <thread>
#include <atomic>

using namespace std;

std::atomic<bool> inputReceived = false;

void getInput(std::string &input) {
    std::cin >> input;
    inputReceived = true;
}

void menu();

string space_to_underscore(string str) {
    for (int i = 0; i < str.length(); i++) {
        if (str[i] == ' ') {
            str[i] = '_';
        }
    }
    return str;
}

string add_aspas(string str) {
    string new_str = "\"";
    for (int i = 0; i < str.length(); i++) {
        new_str += str[i];
    }
    new_str += "\"";
    return new_str;
}

void enviar() {
    string nome = "Bolinhas"; // Replace with the desired name
    string mensagem;

    cout << "Digite sua mensagem: ";
    std::getline(std::cin >> std::ws, mensagem);

    string mensagem_ = space_to_underscore(mensagem);
    string url = "127.0.0.1:8000/enviar?nome='" + nome + "|" + mensagem_ + "'";

    string post_data = "";
    stringstream response_stream;
    url = add_aspas(url);
    string command = "curl -s -X POST -d \"" + post_data + "\" " + url;
    FILE *pipe = popen(command.c_str(), "r");

    if (!pipe) {
        cerr << "Failed to run 'curl' command." << endl;
        return;
    }

    char buffer[128];
    while (!feof(pipe)) {
        if (fgets(buffer, 128, pipe) != NULL) {
            response_stream << buffer;
        }
    }

    string response = response_stream.str();
    pclose(pipe);
}

vector<string> split(const string &input, char delimiter) {
    vector<string> tokens;
    size_t start = 0;
    size_t end = input.find(delimiter);

    while (end != string::npos) {
        tokens.push_back(input.substr(start, end - start));
        start = end + 1;
        end = input.find(delimiter, start);
    }

    tokens.push_back(input.substr(start, end));
    return tokens;
}

string fetch() {
    string content = "";
    string url = "127.0.0.1:8000"; // Replace with your target URL
    string post_data = "";
    string command = "curl -s -X POST -d \"" + post_data + "\" " + url;
    FILE *pipe = popen(command.c_str(), "r");

    if (!pipe) {
        cerr << "Failed to run 'curl' command." << endl;
    }

    char buffer[128];
    while (!feof(pipe)) {
        if (fgets(buffer, 128, pipe) != NULL) {
            content += buffer;
        }
    }

    pclose(pipe);
    return content;
}

void mostrar() {
    try {
        string content = fetch();
        vector<string> messages = split(content, '}');

        for (const string &message: messages) {
            if (!message.empty()) {
                size_t nameStart = message.find("\"nome\":\"") + 8;
                size_t nameEnd = message.find("\",", nameStart);
                string name = message.substr(nameStart, nameEnd - nameStart);

                size_t contentStart = message.find("\"content\":\"") + 11;
                size_t contentEnd = message.find("\"}", contentStart);
                string messageContent = message.substr(contentStart, contentEnd - contentStart);
                messageContent.pop_back();

                cout << name << ": " << messageContent << endl;
            }
        }
    } catch (exception &e) {
        cout << endl;
    }
}

void apagar() {
    try {
        string url = "127.0.0.1:8000/apagar";
        string post_data = "";
        stringstream response_stream;
        string command = "curl -s -X POST -d \"" + post_data + "\" " + url;
        FILE *pipe = popen(command.c_str(), "r");

        if (!pipe) {
            cerr << "Failed to run 'curl' command." << endl;
            return;
        }

        char buffer[128];
        while (!feof(pipe)) {
            if (fgets(buffer, 128, pipe) != NULL) {
                response_stream << buffer;
            }
        }

        string response = response_stream.str();
        cout << "Todas as mensagens foram apagadas!" << endl;
        menu();
        pclose(pipe);
    } catch (exception &e) {
        menu();
    }
}

int main() {
    string a;
    cout << "Digite 'e' para enviar mensagens, 'r' para dar reload, e 'a' para apagar as mensagens" << endl;
    cout << "Pressione qualquer tecla para continuar" << endl;
    cin >> a;
    menu();
    return 0;
}

void menu() {

    string input;

    system("clear");
    mostrar();

    cin >> input;


    if (input == "r") {
        menu();
    } else if (input == "e") {
        enviar();
        menu();
    } else if (input == "a") {
        apagar();
        menu();
    } else {
        menu();
    }

    system("clear");
}
