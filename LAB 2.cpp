// Lab
// Name           : Corey A. Waldon
// Class          : COMP 2710 Section 001
// Date           : 10/20/2017
// Email          : caw0086@auburn.edu
// Auburn User ID : caw0086
//
// Description:    Lab 2
#include <iostream>
#include <vector>
#include <map>
#include <queue>
#include <set>
#include <fstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <sstream>
#include <algorithm>

#include <unistd.h>
using namespace std;

// Class Declarations
class IOManager;
class MessageBuffer;
class User;
class Group;
class Menu;
class System;

class System {
private:
    Menu *menu;
public:
    System();
};

int main() {
    System();
    return 0;
}

// Function    :
// Inputs      :
// Outputs     :
// Description :
string format_title(string msg) {
    string output = " ";
    for (int i = 0; i < msg.length() + 18; i++) {
        output += "=";
    }
    output += " \n|         " + msg + "         |\n ";
    for (int i = 0; i < msg.length() + 18; i++) {
        output += "=";
    }
    return output;
}

string get_epoch() {
    time_t t = time(0);
    stringstream ss;
    ss << t;
    string ts = ss.str();
    return ts;
}

class User {
private:
    set<string> group_names;
    string username;
public:
    User(string username) : username(username) { }

    // Function    : get_username
    // Inputs      : N/A
    // Outputs     : string - The username of the user
    // Description : Returns the username of the user.
    string get_username() {
        return username;
    }

    // Function    : add_group
    // Inputs      : string group - The name of group the user is being added to
    // Outputs     : N/A
    // Description : Adds the group to the user's group list
    void add_group(string group) {
        group_names.emplace(group);
    }

    // Function    : get_group_list
    // Inputs      : N/A
    // Outputs     : set<string> - The set of all the group names corresponding to groups that the user belongs to.
    // Description : Returns the list of groups a user belongs to.
    set<string> get_group_list() {
        return group_names;
    }

    // Function    : print_user_details
    // Inputs      : N/A
    // Outputs     : N/A
    // Description : Prints the groups a user belongs to.
    void print_user_details() {
        cout << endl << format_title(username + "'s Groups") << endl;
        for (auto it = group_names.begin(); it != group_names.end(); it++) {
            cout << "|         " << *it << endl;
        }
    }
};

class Group {
private:
    string group_name;
    set<User *> users;
public:
    Group(string group_name) : group_name(group_name) { }

    // Function    : get_group_name
    // Inputs      : N/A
    // Outputs     : string - The group's name.
    // Description : Returns the group's name.
    string get_group_name() {
        return group_name;
    }


    // Function    : add_user
    // Inputs      : User *user - The user being added
    // Outputs     : N/A
    // Description : Adds a user to the group
    void add_user(User *user) {
        users.emplace(user);
    }

    // Function    : get_users()
    // Inputs      : N/A
    // Outputs     : set<User *> - All users in the group
    // Description : Retrieves all users in the group.
    set<User *> get_users() {
        return users;
    }
};

class MessageBuffer {
private:
    deque<string> messages;

    // Function    : fmt
    // Inputs      : User * op      - The user who posted the message
    //             : Group * group  - The group the message was posted in.
    //             : string message - The message
    // Outputs     : string         - The formatted message
    // Description : Formats the message to a string as specified by the Lab 1 pdf.
    string fmt(User *op, Group *group, string message) {

        return "{!" + get_epoch() + "!}<!" + op->get_username() + ":" + group->get_group_name() + "!>" + message;
    }

    string pull_epoch(string fmt_msg_w_t) {
        return fmt_msg_w_t.substr(2, fmt_msg_w_t.find("!}" - 2));
    }

public:
    MessageBuffer(){ }

    // Function    : prepend_message
    // Inputs      : User * op      - The user who posted the message
    //               Group * group  - The group the message was posted in.
    //               string message - The message to be prepended.
    // Outputs     : N/A
    // Description : Prepends the message (formatted) to the message buffer.
    void prepend_message(User *op, Group *group, string message) {
        messages.push_front(fmt(op, group, message));
    }

    // Function    : prepend_message
    // Inputs      : string fmt_msg - A formatted message
    // Outputs     : N/A
    // Description : Prepends the message (formatted) to the message buffer.
    void prepend_message(string fmt_msg) {
        messages.push_front(fmt_msg);
    }

    bool contains_message(string fmt_msg) {
        for (string message : messages) {
            if (message == fmt_msg)
                return true;
        }
        return false;
    }

    // Function    : add_message
    // Inputs      : string fmt_msg_w_t - A formatted message with a time stamp
    // Outputs     : N/A
    // Description : Adds the message to the message buffer in the proper location.
    void add_message(string fmt_msg_w_t) {
        if (!contains_message(fmt_msg_w_t)) {
            string epoch = pull_epoch(fmt_msg_w_t);
            if (messages.empty())
                messages.push_front(fmt_msg_w_t);
            else
                messages.push_front(fmt_msg_w_t);
            sort(messages.rbegin(), messages.rend());
        }
    }

    // Function    : get_messages_by_user
    // Inputs      : string username - The username for lookup
    // Outputs     : queue<string>   - The messages found by the lookup.
    // Description : Does a linear scan of the buffer to find messages sent by a user.
    queue<string> get_messages_by_user(string username) {
        queue<string> output;
        for (string message : messages) {
            string m = rip_epoch(message);
            m = m.substr(2, m.find(":") - 2);
            if (m == username)
                output.push(message);
        }
        return output;
    }

    // Function    : get_messages_by_groups
    // Inputs      : string username  - The username for lookup
    //               vector <Group *> - The groups a user belongs to
    // Outputs     : queue<string>    - The messages found by the lookup.
    // Description : Does a linear scan of the buffer to find messages sent to that user or groups the user belongs to.
    queue<string> get_messages_by_groups(string username, vector<Group *> groups) {
        queue<string> output;
        for (string message : messages) {
            string m = message, u = message;
            m = m.substr(m.find(":") + 1, m.find("!>") - m.find(":") - 1);
            u = u.substr(m.find("!}") + 2, m.find(":") - (m.find("!}") + 2));
            for (Group *group : groups)
                if (group->get_group_name() == m || username == m || u == username) {
                    output.push(message);
                    break;
                }
        }
        return output;
    }

    string get_messages_of_group(string group_name) {
        string output;

        group_name = (group_name.at(0) != '_' ? group_name : "#" + group_name.substr(1, group_name.length()));
        for (string message : messages) {
            string m = message, u = message;
            m = m.substr(
                    m.find(group_name.at(0) == '#' ? ":#" : ":") + 1, m.find("!>") -
                    m.find(group_name.at(0) == '#' ? ":#" : ":") - 1);
            if (m == group_name)
                output += strip_group(message);
        }
        return output;
    }

    string strip_group(string fmt_msg_w_t) {
        string n = fmt_msg_w_t.substr(0, fmt_msg_w_t.find(":")) + fmt_msg_w_t.substr(fmt_msg_w_t.find("!>"));
        return n;
    }

    string rip_epoch(string fmt_msg_w_t) {
        return fmt_msg_w_t.substr(fmt_msg_w_t.find("!}") + 2);
    }
};

class Network {
private:
    vector<User *> users;
    vector<Group *> groups;
    
public:

    Network() {
        groups.push_back(new Group("#All"));
    }

    // Function    : get_user
    // Inputs      : string username - The username for lookup.
    // Outputs     : User *          - The user corresponding to that username on the network. NULL is username DNE.
    // Description : Returns the user corresponding to the username.
    User *get_user(string username) {
        for (User *u : users) {
            if (u->get_username() == username) {
                return u;
            }
        }
        users.push_back(new User(username));
        return get_user(username);
    }

    // Function    : has_user
    // Inputs      : User user - The user for lookup.
    // Outputs     : bool      - Whether or not the user exists on the network.
    // Description : Does a linear scan of all the users to check to see if the one being looked up exists in the
    //               network.
    bool has_user(User user) {
        for (User *u : users) {
            if (u->get_username() == user.get_username()) {
                return true;
            }
        }
        return false;
    }

    // Function    : add_user
    // Inputs      : User * user - The user being added.
    // Outputs     : bool        - Whether or not the user was added successfully.
    // Description : Adds a user to the network. Returns true/false depending on whether or not the user was
    //               successfully added.
    bool add_user(User *user) {
        if (!has_user(*user))
            users.push_back(user);
        else return false;
        return true;
    }

    // Function    : has_group
    // Inputs      : Group &group - The group for lookup
    // Outputs     : bool         - Whether or not the group exists on the network.
    // Description : Does a linear scan of all the groups to check to see if the one being looked up exists in the
    //               network.
    bool has_group(Group &group) {
        for (Group *g : groups) {
            if (g->get_group_name() == group.get_group_name()) {
                return true;
            }
        }
        return false;
    }

    // Function    : has_group
    // Inputs      : Group &group - The group for lookup
    // Outputs     : bool         - Whether or not the group exists on the network.
    // Description : Does a linear scan of all the groups to check to see if the one being looked up exists in the
    //               network.
    bool has_group(Group *group) {
        for (Group *g : groups) {
            if (g->get_group_name() == group->get_group_name()) {
                return true;
            }
        }
        return false;
    }

    // Function    : add_group
    // Inputs      : Group * group - The group being added.
    // Outputs     : bool          - Whether or not the group was added successfully.
    // Description : Adds a group to the network. Returns true/false depending on whether or not the group already
    //               existed in the network.
    bool add_group(Group *group) {
        if (!has_group(*group))
            groups.push_back(group);
        else return false;
        return true;
    }

    // Function    : get_group
    // Inputs      : string group_name - The name of the group being retrieved.
    // Outputs     : Group *           - The group corresponding to the group_name lookup.
    // Description : Retrieves a pointer to a Group corresponding to a group name.
    Group *get_group(string group_name) {
        for (Group *g : groups) {
            if (g->get_group_name() == group_name)
                return g;
        }
        Group *g =  new Group(group_name);
        return g;
    }

    // Function    : private_group
    // Inputs      : string recipient - The username being unicasted to.
    // Outputs     : Group *         - The newly created pointer to a Group.
    // Description : Creates a new "group" meant to handle unicast messages.
    Group *private_group(string recipient) {
        Group *group = new Group(recipient);
        if (has_group(*group)) {
            group = get_group(recipient);
            group->add_user(get_user(recipient));
            return group;
        }
        groups.push_back(group);
        group->add_user(get_user(recipient));
        return group;
    }

    // Function    : get_groups_of_user
    // Inputs      : User user       - The user for lookup.
    // Outputs     : vector<Group *> - The list of groups associated with a the user.
    // Description : Gets all of the groups a user is in and returns them.
    vector<Group*> get_groups_of_user(User user) {
        vector<Group*> output;
        set<string> grouplist = user.get_group_list();
        for (auto it = grouplist.begin(); it != grouplist.end(); it++) {
            output.push_back(get_group(*it));
        }
        return output;
    }

    // Function    : get_user_count
    // Inputs      : N/A
    // Outputs     : int - Total number of users in the network
    // Description : Returns the total number of users in the network.
    int get_user_count() {
        return users.size();
    }

    // Function    : get_group_count
    // Inputs      : N/A
    // Outputs     : int - Total number of groups in the network
    // Description : Returns the total number of groups in the network.
    int get_group_count() {
        return groups.size();
    }

    // Function    : list_users
    // Inputs      : N/A
    // Outputs     : N/A
    // Description : Prints out all of the users in the network.
    void list_users() {
        cout << endl << format_title("All Users");
        for (User *user : users)
            cout << endl << "|         " << user->get_username();
        if (users.size() == 0)
            cout << endl << "|         No users found";
        cout << endl;
    }

    // Function    : list_groups
    // Inputs      : N/A
    // Outputs     : N/A
    // Description : Prints out all of the groups in the network.
    void list_groups() {
        cout << endl << format_title("All Groups");
        for (Group *group : groups)
            cout << endl << "|         " << group->get_group_name();
        cout << endl;
    }

    vector<User*> get_all_users() {
        return users;
    }

    vector<Group*> get_all_groups() {
        return groups;
    }
};

class IOManager {
    string GROUPS = "ds_groups",
            FILES  = "ds_files";

    map<string, time_t> file_mods;

    set<string> file_names;
private:

    MessageBuffer *buffer;
    Network *network;


    // Function    : update_available()
    // Inputs      : N/A
    // Outputs     : bool - If there was an update available for any of the files.
    // Description : Checks if any file has been changed since it was last read from.
    bool update_available() {
        if (!file_names.empty()) {
            int x = file_names.size();
            for (string file : file_names)
                if (file_changed(file))
                    return true;
        }
        return file_changed(FILES) || file_changed(GROUPS);
    }

    // Function    : file_changed
    // Inputs      : string file_name - The name of file
    // Outputs     : bool - If there was an update available for this file.
    // Description : Checks if the file has been changed since it was last read from.
    bool file_changed(string file_name) {
        struct stat result;
        stat(file_name.c_str(), &result);
        if (result.st_mtime > file_mods[file_name]) {
            file_mods[file_name] = result.st_mtime;
            return true;
        }
        return false;
    }

    // Function    : format_file_data
    // Inputs      : string FILE_NAME - The name of file
    // Outputs     : N/A
    // Description : Formats the data for the file using in-program memory.
    string format_file_data(string FILE_NAME) {
        int type = -1;
        if (FILE_NAME == GROUPS) type = 0;
        if (FILE_NAME == FILES) type = 1;
        string output;
        switch(type) {
            case 0: {
                for (Group *group : network->get_all_groups()) {
                    output += group->get_group_name() + ":";
                    for (User *member : group->get_users())
                        output += member->get_username() + ";";
                    output += "\n";
                }
                return output;
            }
            case 1: {
                for (Group *group : network->get_all_groups())
                    output += (group->get_group_name().at(0) != '#' ? "" : "_") + group->get_group_name().substr(group->get_group_name().at(0) != '#' ? 0 : 1, group->get_group_name().length()) + "\n";
                return output;
            }
            default: {
                output += buffer->get_messages_of_group(FILE_NAME);
                return output;
            }
        }
    }

    // Function    : parse_data
    // Inputs      : int type     - The type of file being read
    //               ifstream &in - The file being read from
    // Outputs     : N/A
    // Description : Reads the information from the disk and puts it in memory.
    void parse_data(int type, ifstream &in, string group) {
        string input;
        switch (type) {
            case 0: { // GROUPS
                while (getline(in, input)) {
                    string group_name = input.substr(0, input.find(":"));
                    input = input.substr(input.find(":") + 1, input.length() - input.find(":"));
                    Group *g = group_name == "#All" ? network->get_group(group_name) :  new Group(group_name);
                    if (!network->has_group(g))
                        network->add_group(g);
                    while (input.find(";") != string::npos) {
                        string user = input.substr(0, input.find(";"));
                        input = input.substr(input.find(";") + 1);
                        g->add_user(network->get_user(user));
                        network->get_user(user)->add_group(group_name);
                    }
                }
                break;
            }
            case 1: { // FILES
                while(getline(in, input)) {
                    file_names.emplace(input);
                }
                break;
            }
            case 2: { // CAST
                if (group.at(0) == '_')
                    group = "#" + group.substr(1);
                string message;
                while(getline(in, input))
                   message += input + "\n";
                vector<string> messages = split(message, '{');
                for (string msg : messages) {
                    msg = "{" + msg;
                    msg = msg.substr(0, msg.find("!>")) + ":" + group +  msg.substr(msg.find("!>"));
                    buffer->add_message(msg);
                }
                break;
            }
            default:
                break;
        }
        in.close();
    }

    // Function    : parse_data
    // Inputs      : const string& s - The string to tokenize
    //               char delimiter  - The delimiter
    // Outputs     : vector<string>
    // Description : Tokenizes a string into a vector of tokens
    vector<string> split(const string& s, char delimiter) {
        vector<string> tokens;
        string token;
        istringstream tokenStream(s);
        while (getline(tokenStream, token, delimiter))
            if (token != "")
                tokens.push_back(token);
        return tokens;
    }

public:

    // Function    : update
    // Inputs      : N/A
    // Outputs     : N/A
    // Description : Checks to see if there is an update, and if there is, downloads it.
    void update() {
        if (update_available()) {
            for (int file = 0; file < 3; file++) {
                string input;
                ifstream in;
                switch (file) {
                    case 0: {
                        in.open(GROUPS);
                        break;
                    }
                    case 1: {
                        in.open(FILES);
                        break;
                    }
                    default: {
                        for(string file_name : file_names) {
                            in.open(file_name);
                            parse_data(2, in, file_name);
                        }
                        return;
                    }

                }

                parse_data(file, in, "");
            }
        }
    }

    // Function    : write_changes()
    // Inputs      : N/A
    // Outputs     : N/A
    // Description : Writes all the files to the disk.
    void write_changes() {
        update();
        ofstream g, f;
        g.open(GROUPS);
        g << format_file_data(GROUPS);
        g.flush();
        f.open(FILES);
        f << format_file_data(FILES);
        f.flush();
        ofstream *out;
        for (string file : file_names) {
            out = new ofstream();
            out->open(file);
            string data = format_file_data(file);
            *out << data;
            out->close();
            out->clear();
        }
        update();
    }

    IOManager (MessageBuffer *buffer, Network *network) {
        this->buffer = buffer;
        this->network = network;
        file_names.emplace("_All");
    }
};

class Menu {
private:
    User *current_user;
    IOManager *manager;

    // Function    : init
    // Inputs      : N/A
    // Outputs     : N/A
    // Description : Runs the main loop of the program, prompting for input and running the appropriate functions
    //               given the input.
    void init() {
        manager = new IOManager(buffer, network);
        cout << format_title("Distributed TigerBook System!");
        manager->update();
        cout << "\n\nPlease enter your user name: ";
        string name;
        getline(cin, name);
        new_user(name);
        cout << "\n\n" << format_title("Welcome to Distributed TigerBook System, " + name + "!");
        while (1) {
            cout << "\n\nBroadcast (b), "
                    "Multicast (m), "
                    "Unicast (u), "
                    "Wall page (w), "
                    "Home page (h), "
                    "Create new group (g), "
                    "Join a group(j), "
                    "List users (ul), "
                    "List groups (gl), "
                    "Get user details (d), "
                    "Quit(q)\n";
            cout << (current_user != NULL ? "\n" + current_user->get_username() + ", p" : "\nP") + "lease choose an option: ";
            string option;
            getline(cin, option);
            manager->update();
            if        (option == "b"  || option == "B") {
                broadcast();
            } else if (option == "m"  || option == "M") {
                multicast();
            } else if (option == "u"  || option == "U") {
                unicast();
            } else if (option == "w"  || option == "W") {
                wall_page();
            } else if (option == "h"  || option == "H") {
                home_page();
            } else if (option == "g"  || option == "G") {
                new_group();
            } else if (option == "j"  || option == "J") {
                join_group();
            } else if (option == "ul" || option == "UL") {
                list_users();
            } else if (option == "gl" || option == "GL") {
                list_groups();
            } else if (option == "d"  || option == "D") {
                if (current_user != NULL)
                    current_user->print_user_details();
                    current_user->print_user_details();
            } else if (option == "q"  || option == "Q") {
                cout << format_title("Thank you for using the Distributed TigerBook System!");
                break;
            }
            manager->write_changes();
        }
    }

    // Function    : poll_message
    // Inputs      : N/A
    // Outputs     : N/A
    // Description : Continuously prompts the user for a message until exit with ^!.
    string poll_message() {
        cout << "\nEnter message: ";
        string message, line;
        while (1) {
            getline(cin, line);
            if (line != "^!")
                message += line + "\n";
            else {
                message = message.substr(0, message.length() - 1);
                break;
            }
        }
        return message;
    }

    // Function    : format_msg
    // Inputs      : bool   is_wall - Is the formatted message for the Wall or Home page?
    //               string msg     - The message to be formatted.
    // Outputs     : string         - The formatted variant of the string
    // Description : Formats the message according to the page it is meant to be displayed on.
    string format_msg(bool is_wall, string msg) {
        msg = buffer->rip_epoch(msg);
        string username = msg.substr(2, msg.find(":") - 2);
        string group = msg.substr(3 + username.length(), msg.find("!>") - (3 + username.length()));
        string message = msg.substr(5 + username.length() + group.length());
        return (!is_wall ? username + " " : "") + "(" + group + ") >\n" + message;
    }

    // Function    : print_message_queue
    // Inputs      : bool          is_wall  - Is the formatted message for the Wall or Home page?
    //               queue<string> messages - The messages to be printed
    // Outputs     : N/A
    // Description : Prints all of the messages in a queue in the pattern specified by the Lab 1 pdf.
    void print_message_queue(bool is_wall, queue<string> messages) {
        int count = min((int) messages.size(), 2);
        for (int i = 0; i < count; i++) {
            cout << format_msg(is_wall, messages.front()) << endl;
            messages.pop();
        }
        if (!messages.empty()) {
            cout << "              More message? (yes/no) : ";
            string response;
            getline(cin, response);
            while (response != "yes" && response != "no") {
                cout << "              Error! Invalid response.\n             Please enter yes or no : ";
                getline(cin, response);
            }
            cout << endl;
            if (response == "yes") {
                while (!messages.empty()) {
                    cout << format_msg(is_wall, messages.front()) << endl;
                    messages.pop();
                }
            }
        }
    }

    // Function    : new_user
    // Inputs      : N/A
    // Outputs     : N/A
    // Description : Creates a new user and adds them to the #All channel and sets them as the current user.
    void new_user() {
        cout << "\nPlease enter user name: ";
        string username;
        getline(cin, username);
        User *temp = new User(username);
        if (!network->add_user(temp)) {
            cout << "\nError! User already exists.";
            new_user();
        } else {
            cout << endl << format_title("Welcome to TigerBook Social Network, " + username + "!");
        }
        network->add_user(temp);
        temp->add_group("#All");
        current_user = temp;
    }

    // Function    : new_user
    // Inputs      : string username
    // Outputs     : N/A
    // Description : Creates a new user and adds them to the #All channel and sets them as the current user.
    void new_user(string username) {
        User *temp = network->get_user(username);
        temp->add_group("#All");
        network->get_group("#All")->add_user(temp);
        current_user = temp;
    }

    // Function    : broadcast
    // Inputs      : N/A
    // Outputs     : N/A
    // Description : Broadcasts a message to the #All channel.
    void broadcast() {
        if (current_user != NULL) {
            string message = poll_message();
            buffer->prepend_message(current_user, network->get_group("#All"), message);
        } else
            cout << "\nError! No users exist.";
        cout << endl << endl << format_title(current_user->get_username() + " has broadcasted a new message");
    }

    // Function    : multicast
    // Inputs      : N/A
    // Outputs     : N/A
    // Description : Multicasts a message to a specified group.
    void multicast() {
        if (current_user != NULL) {
            cout << "\nEnter the group name: ";
            string group;
            getline(cin, group);
            Group g = Group(group);
            while (!network->has_group(g)) {
                cout << "Error! Not a valid group.\nEnter the group name: ";
                getline(cin, group);
                g = Group(group);
            }
            string message = poll_message();
            buffer->prepend_message(current_user, network->get_group(group), message);
        } else
            cout << "\nError! No users exist.";
    }

    // Function    : unicast
    // Inputs      : N/A
    // Outputs     : N/A
    // Description : Unicasts a message to a specified user.
    void unicast() {
        if (current_user != NULL) {
            if (network->get_user_count() > 1) {
                cout << "\nEnter the recipient user name: ";
                string username;
                getline(cin, username);
                User user = User(username);
                while (!network->has_user(user)) {
                    cout << "\nError! Not a valid username.\nEnter the recipient user name: ";
                    getline(cin, username);
                    user = User(username);
                }
                string message = poll_message();
                buffer->prepend_message(current_user, network->private_group(username), message);
            } else
                cout << "\nError! No other users to send messages to.";
        } else
            cout << "\nError! No users exist.";
        cout << endl << endl << format_title(current_user->get_username() + " has unicasted a new message");
    }

    // Function    : wall_page
    // Inputs      : N/A
    // Outputs     : N/A
    // Description : Displays the current user's wall page.
    void wall_page() {
        if (current_user != NULL) {
            cout << endl << format_title(current_user->get_username() + "'s Wall Page") << endl << endl;
            queue<string> messages = buffer->get_messages_by_user(current_user->get_username());
            print_message_queue(true, messages);
        } else
            cout << "\nError! No users exist.";
    }

    // Function    : home_page
    // Inputs      : N/A
    // Outputs     : N/A
    // Description : Displays the current user's home page.
    void home_page() {
        if (current_user != NULL) {
            cout << endl << format_title(current_user->get_username() + "'s Home Page") << endl << endl;
            queue<string> messages = buffer->get_messages_by_groups(current_user->get_username(),
                                                                    network->get_groups_of_user(*current_user));
            print_message_queue(false, messages);
        } else
            cout << "\nError! No users exist.";
    }

    // Function    : new_group
    // Inputs      : N/A
    // Outputs     : N/A
    // Description : Creates a new group with the specified name.
    void new_group() {
        cout << "\nEnter group name: ";
        string groupname;
        getline(cin, groupname);
        Group *group = new Group(groupname);
        while (network->has_group(*group)) {
            cout << "\nError! Group already exists.\nEnter group name: ";
            getline(cin, groupname);
            delete(group);
            group = new Group(groupname);
        }
        network->add_group(group);
        cout << endl << format_title(group->get_group_name() + " group created");
    }

    // Function    : join_group
    // Inputs      : N/A
    // Outputs     : N/A
    // Description : Adds the current user to the specified group.
    void join_group() {
        if (current_user != NULL) {
            if (network->get_group_count() != current_user->get_group_list().size()) {
                cout << "\nEnter group name: ";
                string group_name;
                getline(cin, group_name);
                Group *group = new Group(group_name);
                while (!network->has_group(*group)) {
                    cout << "\nError! Group does not exist.\nEnter group name: ";
                    getline(cin, group_name);
                    delete (group);
                    group = new Group(group_name);
                }
                Group *group_p = network->get_group(group_name);
                current_user->add_group(group_p->get_group_name());
                group_p->add_user(current_user);
                cout << endl << format_title(current_user->get_username() + " has joined group" + group_name);
            } else
                cout << "\nError! No other groups to join";
        } else
            cout << "\nError! No users exist.";
    }

    // Function    : switch_user
    // Inputs      : N/A
    // Outputs     : N/A
    // Description : Switches the current user to the specified user.
    void switch_user() {
        cout << "\nEnter user's name: ";
        string username;
        getline (cin, username);
        User user = User(username);
        while (!network->has_user(user)) {
            "\nError! Not a valid username.\nEnter user's name: ";
            getline(cin, username);
            user = User(username);
        }
        current_user = network->get_user(username);
        cout << endl << format_title("Welcome back to TigerBook Social Network, " + username + "!");
    }

    // Function    : list_users
    // Inputs      : N/A
    // Outputs     : N/A
    // Description : Lists all of the users in the network.
    void list_users() {
        network->list_users();
    }

    // Function    : list_groups
    // Inputs      : N/A
    // Outputs     : N/A
    // Description : Lists all of the groups in the network.
    void list_groups() {
        network->list_groups();
    }

public:
    Network *network;
    MessageBuffer *buffer;
    System *system;

    Menu(System *system, Network *network, User *currentuser) : system(system), network(network), current_user(currentuser) {
        buffer = new MessageBuffer();
        init();
    }
};



System::System() {
    menu = new Menu(this, new Network(),  NULL);
}