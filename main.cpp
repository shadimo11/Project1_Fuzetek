#include <iostream>
#include <vector>
#include <string>
#include <ctime>
using namespace std;

// ========================
//       USER CLASS
// ========================
class User
{
private:
    string username;
    string password;
    string phoneNumber;
    string status;
    string lastSeen;

public:
    User()
    {
        username="";
        password="";
        phoneNumber="";
        status="";
        time_t timestamp =time(nullptr);
        lastSeen=ctime(&timestamp);

    }

    User(string uname, string pwd, string phone)
    {

        username=uname;
        password=pwd;
        phoneNumber=phone;
        status="";
        time_t timestamp =time(nullptr);
        lastSeen=ctime(&timestamp);

    }

    string getUsername() const
    {
        return username;
    }

    string getPhoneNumber() const
    {
        return phoneNumber;
    }

    string getStatus() const
    {
        return status;
    }

    string getLastSeen() const
    {
        return lastSeen;
    }

    void setStatus(string newStatus)
    {
        status = newStatus;
    }

    void setPhoneNumber(string phone)
    {
        phoneNumber = phone;
    }

    void updateLastSeen()
    {
        time_t timestamp =time(nullptr);
        lastSeen=ctime(&timestamp);

    }

    bool checkPassword(string pwd) const
    {
        if (pwd == password)
            return true;
        return false;
    }

    void changePassword(string newPwd)
    {
        password = newPwd;
    }
};

// ========================
//      MESSAGE CLASS
// ========================
class Message
{
private:
    string sender;
    string content;
    string timestamp;
    string status;
    Message* replyTo;

public:
    Message()
    {
        sender = "";
        content = "";
        timestamp = "";
        status = "sent";
        replyTo = nullptr;
    }

    Message(string sndr, string cntnt)
    {
        sender = sndr;
        content = cntnt;

        time_t current_time = time(nullptr);
        timestamp = ctime(&current_time);

        status = "sent";
        replyTo = nullptr;
    }

    string getContent() const
    {
        return content;

    }

    string getSender() const
    {
        return sender;

    }

    string getTimestamp() const
    {
        return timestamp;

    }

    string getStatus() const
    {
        return status;

    }

    Message* getReplyTo() const
    {
        return replyTo;
    }

    void setStatus(string newStatus)
    {
        status=newStatus;
    }

    void setReplyTo(Message* msg)
    {
        replyTo=msg;
    }

    void updateTimestamp()
    {
        timestamp = time(0);
    }

    void display() const
    {
        cout << "Sender:" << sender << endl;
        cout << "Message:" << content << endl;
        cout << "Time:" << timestamp << endl;
        cout << "Status:" << status << endl;
    }

    void addEmoji(string emojiCode)
    {
        string from = emojiCode;
        string to = "";
        if (emojiCode == ":(")
            to = "😔";
        else if (emojiCode == ":)")
            to = "😊";
        else if (emojiCode == ":D")
            to = "😀";
        else if (emojiCode == "<3")
            to = "❤️";
        else if (emojiCode == ":thumbsup")
            to = "👍";
        else
            cout << "[!] Unknown emoji code: " << emojiCode << endl;
    }
};

// ========================
//       CHAT CLASS (BASE)
// ========================
class Chat
{
protected:
    vector<string> participants;
    vector<Message> messages;
    string chatName;

public:
    Chat()
    {
        chatName = "";
    }

    Chat(vector<string> users, string name)
    {
        participants = users;
        chatName = name;
    }

    void addMessage(const Message& msg)
    {
        messages.push_back(msg);
    }

    bool deleteMessage(int index, const string& username)
    {
        if(index>=0 && index<messages.size())
        {
            if(messages[index].getStatus()=="sent" && messages[index].getSender()==username)
            {
                messages.erase(messages.begin()+index);
                return true;
            }
            else
            {
                cout<<"Username is incorrect or not a sender";
            }
        }
        else
        {
            cout<<"can't delete index out of range";
        }
        return false;
    }

    virtual void displayChat() const
    {
        if (messages.empty())
            cout << "(No messages yet)" << endl;
        else
        {
            cout<<"Messages:"<<endl;
            for(int i=0; i<messages.size(); i++)
            {
                messages[i].display();
            }
            cout << endl;


        }
    }

    vector<Message> searchMessages(string keyword) const
    {
        vector <Message> result;
        for (int i = 0; i < messages.size(); i++)
        {
            if (messages[i].getContent().find(keyword) != string::npos)
            {
                result.push_back(messages[i]);
            }
        }
        return result;
    }

    void exportToFile(const string& filename) const
    {
        // TODO: Implement export to file
    }
};

// ========================
//     PRIVATE CHAT CLASS
// ========================
class PrivateChat : public Chat
{
private:
    string user1;
    string user2;

public:
    PrivateChat(string u1, string u2)
    {
        user1 = u1;
        user2 = u2;
        participants.push_back(u1);
        participants.push_back(u2);
        chatName = "Chat between " + u1 + " and " + u2;
    }

    void displayChat() const override
    {
        cout << " ============ " << chatName << " ============ " << endl;
        if (messages.empty())
        {
            cout << "(No messages yet) " << endl;
        }
        else
        {
            for (const auto &msg : messages)
            {
                msg.display();
            }
        }
        cout << endl;
    }

    void showTypingIndicator(const string& username) const
    {
        cout << " ... " << username << " is typing..." << endl;
    }
};

// ========================
//      GROUP CHAT CLASS
// ========================
class GroupChat : public Chat
{
private:
    vector<string> admins;
    string description;

public:
    GroupChat(vector<string> users, string name, string creator) {
         participants = user;
         groupName = name;
         participants.push_back(creator);
         admins.push_back(creator);
    }

    void addAdmin(string newAdmin)
    {
        if (!isParticipant(newAdmin))
        {
            cout << "User is not in the group." << endl;
            return;
        }
        if (isAdmin(newAdmin))
        {
            cout << newAdmin << " is already an admin." << endl;
            return;
        }
        admins.push_back(newAdmin);
        cout << newAdmin << " is now an admin." << endl;
    }

    bool removeParticipant(const string& admin, const string& userToRemove)
    {
        if (!isAdmin(admin))
        {
            cout << "  [!] Only admins can remove participants." << endl;
            return false;
        }
        for (auto i = participants.begin(); i != participants.end(); i++)
        {
            if (*i == userToRemove)
            {
                participants.erase(i);
                admins.erase(remove(admins.begin(), admins.end(), userToRemove), admins.end());
                cout << "  Done!, " << userToRemove << " removed from the group." << endl;
                return true;
            }
        }
        cout << "  [!] User not found in group." << endl;
        return false;
    }

    bool isAdmin(string username) const {
        for (int i = 0; i < admins.size(); i++)
        {
            if (admins[i] == username)
                return true;
        }
        return false;
    }

    bool isParticipant(string username) const
    {
        for(int i=0; i<participants.size(); i++)
        {
            if(participants[i]==username)
            {
                return true;
            }
        }
        return false;
    }

    void setDescription(string desc) {
        description = desc ;
    }

    void displayChat() const override
    {
        cout << "========== Group: " << chatName << " ==========" << endl;
        if (!description.empty())
            cout << "  Description: " << description << endl;

        cout << "  Participants: ";
        for (int i = 0; i< participants.size(); i++)
        {
            cout << participants[i];
            if (isAdmin(participants[i]))
                cout << " [Admin]";
            if (i < participants.size() - 1)
                cout << ", ";
        }
        cout << endl;

        cout << " ----------------------------------------------" << endl;
        if (messages.empty())
            cout << "  (No messages yet)" << endl;
        else
        {
            for (const auto &msg : messages)
                msg.display();
        }
        cout << endl;
    }

    void sendJoinRequest(const string& username)
    {
        int flag=0;

        for(int i=0; i<participants.size(); i++)
        {
            if(participants[i]==username)
            {
                cout<<"User Already in group";
                flag=1;
                break;
            }
        }
        if(flag==0)
        {
            cout<<"Request is sent sucessfully"
        }


    }
};

// ========================
//    WHATSAPP APP CLASS
// ========================
class WhatsApp
{
private:
    vector<User> users;
    vector<Chat*> chats;
    int currentUserIndex;

    int findUserIndex(string username) const
    {
        for (int i = 0; i < users.size(); i++)
        {
            if (users[i].getUsername() == username)
                return i;
        }
        return -1;
    }

    bool isLoggedIn() const
    {
        return currentUserIndex != -1;
    }

    string getCurrentUsername() const
    {
        if (currentUserIndex == -1)
            return "";
        return users[currentUserIndex].getUsername();
    }

public:
    WhatsApp() : currentUserIndex(-1) {}

    void signUp()
    {
        string uname, pwd, phone;

        cout << "Enter username: ";
        cin >> uname;

        if (findUserIndex(uname) != -1)
        {
            cout << "Username already taken, try another one." << endl;
            return;
        }

        cout << "Enter password: ";
        cin >> pwd;

        if (pwd.length() < 6)
        {
            cout << "Password must be at least 6 characters." << endl;
            return;
        }

        cout << "Enter phone number: ";
        cin >> phone;

        if (phone.empty())
        {
            cout << "Phone number can't be empty." << endl;
            return;
        }

        users.push_back(User(uname, pwd, phone));
        cout << "Account created successfully! You can now log in." << endl;
    }

    void login()
    {
        if (isLoggedIn())
        {
            cout << "You are already logged in as " << getCurrentUsername() << endl;
            return;
        }

        string uname, pwd;

        cout << "Enter username: ";
        cin >> uname;

        cout << "Enter password: ";
        cin >> pwd;

        int index = findUserIndex(uname);

        if (index == -1)
        {
            cout << "Username not found." << endl;
            return;
        }

        if (!users[index].checkPassword(pwd))
        {
            cout << "Wrong password." << endl;
            return;
        }

        currentUserIndex = index;
        users[currentUserIndex].setStatus("Online");
        users[currentUserIndex].updateLastSeen();
        cout << "Welcome back, " << uname << "!" << endl;
    }

    void startPrivateChat()
    {
        // TODO: Implement private chat creation
    }

    void createGroup()
    {
        // TODO: Implement group creation
    }

    void viewChats() const
    {
        // TODO: Implement chat viewing
    }

    void logout()
    {
        {
            if (!isLoggedIn())
            {
                cout << "No one is logged in." << endl;
                return;
            }

            cout << "Goodbye, " << getCurrentUsername() << "!" << endl;
            users[currentUserIndex].setStatus("Offline");
            users[currentUserIndex].updateLastSeen();
            currentUserIndex = -1;
        }
    }

    void run()
    {
        while (true)
        {
            if (!isLoggedIn())
            {
                cout << "\n1. Login\n2. Sign Up\n3. Exit\nChoice: ";
                int choice;
                cin >> choice;

                if (choice == 1) login();
                else if (choice == 2) signUp();
                else if (choice == 3) break;
            }
            else
            {
                cout << "\n1. Start Private Chat\n2. Create Group\n3. View Chats\n4. Logout\nChoice: ";
                int choice;
                cin >> choice;

                if (choice == 1) startPrivateChat();
                else if (choice == 2) createGroup();
                else if (choice == 3) viewChats();
                else if (choice == 4) logout();
            }
        }
    }
};

// ========================
//          MAIN
// ========================
int main()
{
    WhatsApp whatsapp;
    whatsapp.run();
    return 0;

}
