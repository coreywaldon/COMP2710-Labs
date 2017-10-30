// Lab 1
// Name           : Corey A. Waldon
// Class          : COMP 2710 Section 001
// Date           : 10/01/2017
// Email          : caw0086@auburn.edu
// Auburn User ID : caw0086
//
// Description:    Lab 1
#include <iostream>
#include <vector>
#include <map>
#include <queue>
#include <set>
#include <string>

using namespace std;

// Class Declarations
class MessageBuffer;
class User;
class Group;
class Menu;
class System;

class System {
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
    vector<User*> users;
    string group_name;
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
    // Inputs      : User * user - The user being added.
    // Outputs     : N/A
    // Description : Adds the user to the user list.
    void add_user(User *user) {
        users.push_back(user);
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
        return "<!" + op->get_username() + ":" + group->get_group_name() + "!>" + message;
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

    // Function    : get_messages_by_user
    // Inputs      : string username - The username for lookup
    // Outputs     : queue<string>   - The messages found by the lookup.
    // Description : Does a linear scan of the buffer to find messages sent by a user.
    queue<string> get_messages_by_user(string username) {
        queue<string> output;
        for (string message : messages) {
            string m = message;
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
            u = u.substr(2, m.find(":"));
            for (Group *group : groups) {
                if (group->get_group_name() == m || username == m || u == username) {
                    output.push(message);
                    break;
                }
            }
        }
        return output;
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
        return NULL;
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
    Group *getgroup(string group_name) {
        for (Group *g : groups) {
            if (g->get_group_name() == group_name)
                return g;
        }
        return NULL;
    }

    // Function    : private_group
    // Inputs      : string username - The username being unicasted to.
    // Outputs     : Group *         - The newly created pointer to a Group.
    // Description : Creates a new "group" meant to handle unicast messages.
    Group *private_group(string username) {
        Group *group = new Group(username);
        if (has_group(*group))
            return getgroup(username);
        groups.push_back(group);
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
            output.push_back(getgroup(*it));
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


};

class Menu {
private:
    User *currentuser;
    Network *network;
    MessageBuffer *buffer;

    // Function    : init
    // Inputs      : N/A
    // Outputs     : N/A
    // Description : Runs the main loop of the program, prompting for input and running the appropriate functions
    //               given the input.
    void init() {
        buffer = new MessageBuffer;
        cout << format_title("The TigerBook Social Network!");
        while (1) {
            cout << "\n\nCreate a new user (n), "
                    "Broadcast (b), "
                    "Multicast (m), "
                    "Unicast (u), "
                    "Wall page (w), "
                    "Home page (h), "
                    "Create new group (g), "
                    "Join a group(j), "
                    "Switch user (s), "
                    "List users (ul), "
                    "List groups (gl), "
                    "Get user details (d), "
                    "Quit(q)\n";
            cout << (currentuser != NULL ? "\n" + currentuser->get_username() + ", p" : "\nP") + "lease choose an option: ";
            string option;
            getline(cin, option);
            if        (option == "n"  || option == "N") {
                new_user();
            } else if (option == "b"  || option == "B") {
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
            } else if (option == "s"  || option == "S") {
                switch_user();
            } else if (option == "ul" || option == "UL") {
                list_users();
            } else if (option == "gl" || option == "GL") {
                list_groups();
            } else if (option == "d"  || option == "D") {
                if (currentuser != NULL)
                    currentuser->print_user_details();
            } else if (option == "q"  || option == "Q") {
                cout << format_title("Thank you for using TigerBook Social Network!");
                break;
            }
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
            else
                break;
        }
        return message;
    }

    // Function    : format_msg
    // Inputs      : bool   is_wall - Is the formatted message for the Wall or Home page?
    //               string msg     - The message to be formatted.
    // Outputs     : string         - The formatted variant of the string
    // Description : Formats the message according to the page it is meant to be displayed on.
    string format_msg(bool iswall, string msg) {
        string username = msg.substr(2, msg.find(":") - 2);
        string group = msg.substr(3 + username.length(), msg.find("!>") - (3 + username.length()));
        string message = msg.substr(5 + username.length() + group.length());
        return (!iswall ? username + " " : "") + "(" + group + ") >\n" + message;
    }

    // Function    : print_message_queue
    // Inputs      : bool          is_wall  - Is the formatted message for the Wall or Home page?
    //               queue<string> messages - The messages to be printed
    // Outputs     : N/A
    // Description : Prints all of the messages in a queue in the pattern specified by the Lab 1 pdf.
    void print_message_queue(bool iswall, queue<string> messages) {
        int count = min((int) messages.size(), 2);
        for (int i = 0; i < count; i++) {
            cout << format_msg(iswall, messages.front()) << endl;
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
                    cout << format_msg(iswall, messages.front()) << endl;
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
        network->getgroup("#All")->add_user(temp);
        network->add_user(temp);
        temp->add_group("#All");
        currentuser = temp;
    }

    // Function    : broadcast
    // Inputs      : N/A
    // Outputs     : N/A
    // Description : Broadcasts a message to the #All channel.
    void broadcast() {
        if (currentuser != NULL) {
            string message = poll_message();
            buffer->prepend_message(currentuser, network->getgroup("#All"), message);
        } else
            cout << "\nError! No users exist.";
    }

    // Function    : multicast
    // Inputs      : N/A
    // Outputs     : N/A
    // Description : Multicasts a message to a specified group.
    void multicast() {
        if (currentuser != NULL) {
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
            buffer->prepend_message(currentuser, network->getgroup(group), message);
        } else
            cout << "\nError! No users exist.";
    }

    // Function    : unicast
    // Inputs      : N/A
    // Outputs     : N/A
    // Description : Unicasts a message to a specified user.
    void unicast() {
        if (currentuser != NULL) {
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
                buffer->prepend_message(currentuser, network->private_group(username), message);
            } else
                cout << "\nError! No other users to send messages to.";
        } else
            cout << "\nError! No users exist.";
    }

    // Function    : wall_page
    // Inputs      : N/A
    // Outputs     : N/A
    // Description : Displays the current user's wall page.
    void wall_page() {
        if (currentuser != NULL) {
            cout << endl << format_title(currentuser->get_username() + "'s Wall Page") << endl << endl;
            queue<string> messages = buffer->get_messages_by_user(currentuser->get_username());
            print_message_queue(true, messages);
        } else
            cout << "\nError! No users exist.";
    }

    // Function    : home_page
    // Inputs      : N/A
    // Outputs     : N/A
    // Description : Displays the current user's home page.
    void home_page() {
        if (currentuser != NULL) {
            cout << endl << format_title(currentuser->get_username() + "'s Home Page") << endl << endl;
            queue<string> messages = buffer->get_messages_by_groups(currentuser->get_username(),
                                                                    network->get_groups_of_user(*currentuser));
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
        if (currentuser != NULL) {
            if (network->get_group_count() != currentuser->get_group_list().size()) {
                cout << "\nEnter group name: ";
                string groupname;
                getline(cin, groupname);
                Group *group = new Group(groupname);
                while (!network->has_group(*group)) {
                    cout << "\nError! Group does not exist.\nEnter group name: ";
                    getline(cin, groupname);
                    delete (group);
                    group = new Group(groupname);
                }
                Group *group_p = network->getgroup(groupname);
                group_p->add_user(currentuser);
                currentuser->add_group(group_p->get_group_name());
                cout << endl << format_title(currentuser->get_username() + " is in " + groupname + " group");
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
        currentuser = network->get_user(username);
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
    Menu(Network *network, User *currentuser) : network(network), currentuser(currentuser) {
        init();
    }
};

System::System() {
    Menu(new Network(), NULL);
}
