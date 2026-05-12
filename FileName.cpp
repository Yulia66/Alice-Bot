#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <queue>

using namespace std;




class Room {
public:
    int _id;
    size_t _count_res;
    bool _visible;
    bool _visit;
    vector<int> _neighbors;
    vector<int> _resources;
    bool _collected[4];   // какие ресурсы уже собраны
    bool _hasFirstCollect; // был ли уже первый сбор в этой комнате

    Room(int n) {
        for (int i = 0; i < 4; i++) _collected[i] = false;
        _hasFirstCollect = false;
        _id = n;
        _visible = false;
        _visit = false;
    }
    Room() {
        for (int i = 0; i < 4; i++) _collected[i] = false;
        _hasFirstCollect = false;
        _id = -1;
        _visible = false;
        _visit = false;
    }

    void print_res() {
        for (int i = 0; i < 4; i++) {
            cout << _resources[i] << " ";
            
        }
        cout << endl;
    }


};


vector<string> split_by_spaces(const std::string& str) {
    std::istringstream iss(str);
    vector<string> output;
    std::string token;
    while (iss >> token) {
        output.push_back(token);
    }
    return output;
}


vector<int> pars_line(string str) {

    int pos;
    
    if ((pos=str.find(",")) != std::string::npos) {
        str.replace(pos, 1, " ");
    }
    
    vector<string> vec = split_by_spaces(str);
    vector<int> res;
    for (int i = 0; i < vec.size(); i++) {
        int d = std::stoi(vec[i]);
        res.push_back(d);
        //cout << res[i];
    }
    return res;
}

vector<string> divide(string str){
    vector<string> vec;
    while( str.find("\n") != std::string::npos) {
        auto pos = str.find("\n");
        auto token_1 = str.substr(0, pos);
        vec.push_back(token_1);
        auto token_2 = str.substr(pos+1, str.length() - pos + 1);
        str = token_2;
        
    }
    vec.push_back(str);
    return vec;

}





void parser_input(int& gems, size_t& count, vector<Room>& rooms, string str) {
    vector<string> vec = divide(str);
    count = stoi(vec[0]);
    for (int i = 1; i < vec.size()-1; i++) {
       vector<int> res = pars_line(vec[i]);
       Room room;

       room._id = res[0];
       for (int i = res.size() - 1; i > res.size() - 1 - 4; i--) {
           room._resources.push_back(res[i]);
       }
       reverse(room._resources.begin(), room._resources.end());
       for (int i = 1; i < res.size() - 4; i++) {
           room._neighbors.push_back(res[i]);
       }
       
       
       rooms.push_back(room);
       

    }
    gems = stoi(vec[vec.size() - 1]);
}


bool not_end(vector<Room>& rooms) {
    for (Room room : rooms) {
        if (room._visible && !room._visit) {
            return true;
        }
    }
    return false;
}




vector<int> shortestPath_unvisit(int start, vector<Room>& rooms) {
    vector<bool> visited(rooms.size(), false);
    vector<int> parent(rooms.size(), -1);  // запоминаем откуда пришли
    queue<int> q;

    visited[start] = true;
    q.push(start);

    int target = -1;

    while (!q.empty()) {
        int curr = q.front();
        q.pop(); 
       

        for (int neighbor : rooms[curr]._neighbors) {
            // Если дошли до цели
            if (rooms[neighbor]._visible && !rooms[neighbor]._visit) {
                target = neighbor;  // пока просто вернуть номер (потом восстановим путь)
                parent[neighbor] = curr;
                break;
            }
            if (rooms[neighbor]._visit && visited[neighbor] == false) {
                visited[neighbor] = true;
                parent[neighbor] = curr;  // запомнили: в neighbor пришли из curr
                q.push(neighbor);
            }
        }
        if (target != -1) break;
    }
    // Если путь не найден
    if (target == -1) return {};
   
    // Восстанавливаем путь (идем от end к start)
    vector<int> path;
    for (int v = target; v != -1; v = parent[v]) {
        path.push_back(v);
       
    }
    reverse(path.begin(), path.end());  // разворачиваем

    return path;

}


vector<int> shortestPath(int start, int end, vector<Room>& rooms) {
    vector<bool> visited(rooms.size(), false);
    vector<int> parent(rooms.size(), -1);  // запоминаем откуда пришли
    queue<int> q;

    visited[start] = true;
    q.push(start);

    while (!q.empty()) {
        int curr = q.front();
        q.pop();

        // Если дошли до цели
        if (curr == end) break;

        for (int neighbor : rooms[curr]._neighbors) {
            if (!rooms[neighbor]._visit) {
                continue;
            }
            if (!visited[neighbor]) {
                visited[neighbor] = true;
                parent[neighbor] = curr;  // запомнили: в neighbor пришли из curr
                q.push(neighbor);
            }
        }
    }

    // Если путь не найден
    if (!visited[end]) return {};

    // Восстанавливаем путь (идем от end к start)
    vector<int> path;
    for (int v = end; v != -1; v = parent[v]) {
        path.push_back(v);
    }
    reverse(path.begin(), path.end());  // разворачиваем

    return path;
}



int max_res(vector<int> arr) {
    int max = arr[0];
    for (int i = 1; i < arr.size(); i++) {
        if (arr[i] > max) {
            max = arr[i];
        }
    }
    return max;
}




int main() {
    string filename = "in.txt";
    ifstream file(filename);

    if (!file.is_open()) {
        cerr << "Ошибка: не удалось открыть файл" << endl;
        return 1;
    }
    stringstream buffer;
    buffer << file.rdbuf();
    string content = buffer.str();


    size_t count; // колво комнат
    int  food; // ед
    int healf;

    vector<Room> rooms;

    parser_input(food, count, rooms, content);
    healf = food;
    for (int i = 0; i < rooms.size(); i++) {
        for (int neighbor : rooms[i]._neighbors) {
            // Проверить, есть ли уже обратная связь
            bool found = false;
            for (int back : rooms[neighbor]._neighbors) {
                if (back == i) found = true;
            }
            if (!found) {
                rooms[neighbor]._neighbors.push_back(i);
            }
        }
    }

     // фаза исследования
    rooms[0]._visible = true;
    rooms[0]._visit = true;
    for (int neighbor : rooms[0]._neighbors) {
        rooms[neighbor]._visible = true;
    }
    int iron = 0;
    int gold = 0;
    int gems = 0;
    int exp = 0;


    vector<int> resurs = { 7,11,23,1 };

    vector<int> count_res;

    int next_room =-1;
    int cur =0;
    while (not_end(rooms)) {


       


            rooms[cur]._visit = true;


            int bestIdx = -1, bestVal = -1;


            for (int i = 0; i < 4; i++) {
                if (rooms[cur]._resources[i] > 0 && !rooms[cur]._collected[i]) {
                    if (resurs[i] > bestVal) {
                        bestVal = resurs[i];
                        bestIdx = i;
                    }
                }
            }
            if (bestIdx != -1) {
                rooms[cur]._collected[bestIdx] = true;
                // добавляем к счётчику
                if (bestIdx == 0) iron += bestVal;
                if (bestIdx == 1) gold += bestVal;
                if (bestIdx == 2) gems += bestVal;
                if (bestIdx == 3) exp += bestVal;

                int i = rooms[cur]._resources[bestIdx];
                rooms[cur]._resources[bestIdx] = i - 1;
                // Учёт еды
                if (!rooms[cur]._hasFirstCollect) {
                    rooms[cur]._hasFirstCollect = true;   // первый сбор — бесплатно
                }
                else {
                    if (food <= 0) break;
                    food--;   // повторный сбор — платно
                    //if (food <= healf / 2) break;
                }
            }
            next_room = -1;
            for (int nei : rooms[cur]._neighbors) {

                if (!rooms[nei]._visit) {
                    if (next_room == -1 || nei < next_room) {
                        next_room = nei;
                    }

                }
            }

            if (next_room == -1) {
               vector<int> path = shortestPath_unvisit(cur, rooms);
               if (path.size() < 2) {
                   break;
               }
               next_room = path[1];

            }
            if (food <= healf / 2) break;
            cur = next_room;
            food--;
            //if (food <= healf / 2) break;

            if (food < 0) {
                cout << "Еда кончилась, бот умер" << endl;
                break;
            }

            for (int neighbor : rooms[cur]._neighbors) {
                rooms[neighbor]._visible = true;
            }
            
            
            cout << "Перешли в комнату " << cur << ", еды осталось: " << food << " ";
            cout << iron << " " << gold << " " << gems << " " << exp << endl;
        
    }


    // фаза возврата
    vector<int> path;


    
    path = shortestPath(cur, 0, rooms);
    
    size_t size_path = path.size();
    
    for (auto i : path) {
        cur = i;
       // cout << " cur" << " "<< cur;
        if (cur == 0) break;
        while (food > size_path - 1 && food > 0) {
                int bestIdx = -1, bestVal = -1;


                for (int i = 0; i < 4; i++) {
                    if (rooms[cur]._resources[i] > 0 && !rooms[cur]._collected[i]) {
                        if (resurs[i] > bestVal) {
                            bestVal = resurs[i];
                            bestIdx = i;
                        }
                    }
                }
                if (bestIdx != -1) {
                    rooms[cur]._collected[bestIdx] = true;
                    // добавляем к счётчику
                    if (bestIdx == 0) iron += bestVal;
                    if (bestIdx == 1) gold += bestVal;
                    if (bestIdx == 2) gems += bestVal;
                    if (bestIdx == 3) exp += bestVal;

                    int i = rooms[cur]._resources[bestIdx];
                    rooms[cur]._resources[bestIdx] = i - 1;
                    // Учёт еды
                    if (!rooms[cur]._hasFirstCollect) {
                        rooms[cur]._hasFirstCollect = true;   // первый сбор — бесплатно
                    }
                    else {
                        if (food <= 0) break;
                        food--;   // повторный сбор — платно
                        //if (food <= healf / 2) break;
                    }
                }
                cout << "Перешли в комнату " << cur << ", еды осталось: " << food << " ";
                cout << iron << " " << gold << " " << gems << " " << exp << endl;

        }
        size_path--;




    }
    
    file.close();
    return 0;
}


//  неверно парсит осседей 
// не учитывается количесво ресурсов в комнатах