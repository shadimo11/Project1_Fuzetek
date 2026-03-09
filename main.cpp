#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include<algorithm>
#include <windows.h>
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
        time_t t = time(nullptr);
        timestamp = ctime(&t);
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
        else if (emojiCode == ":thumbsup:")
            to = "👍";
        else
            cout << "[!] Unknown emoji code: " << emojiCode << endl;

        if (!to.empty()) {
            size_t pos = 0;
            while ((pos = content.find(from, pos)) != string::npos) {
                content.replace(pos, from.length(), to);
                pos += to.length();
            }
        }
    }
};

// ========================
//       CHAT CLASS (BASE)
// ========================
class Chat
{
    friend class WhatsApp;
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

    bool isParticipant(const string& username) const {
    for (int i = 0; i < (int)participants.size(); i++)
        if (participants[i] == username)
            return true;

    return false;
}

    int getMessageCount() const {
    return (int)messages.size();
    }

    Message* getMessageAt(int index) {
    if (index < 0 || index >= (int)messages.size())
        return nullptr;
    return &messages[index];
    }

    void addMessage(const Message& msg)
    {
        messages.push_back(msg);
    }

    bool deleteMessage(int index, const string& username)
    {
        if(index>=0 && index<messages.size())
        {
            if (messages[index].getSender() == username)
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

    string getChatName() const {
        return chatName;
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
    GroupChat(vector<string> users, string name, string creator)
    {
        participants = users;
        chatName = name;
        participants.push_back(creator);
        admins.push_back(creator);
        description="";
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
                admins.erase(std::remove(admins.begin(), admins.end(), userToRemove), admins.end());
                cout << "  Done!, " << userToRemove << " removed from the group." << endl;
                return true;
            }
        }
        cout << "  [!] User not found in group." << endl;
        return false;
    }

    bool isAdmin(string username) const
    {
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

    void setDescription(string desc)
    {
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
            cout<<"Request is sent sucessfully";
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

    void openChatSession(Chat* chat) {
    while (true) {
        cout << "\n--- " << chat->getChatName() << " ---" << endl;
        cout << "1. Send Message"    << endl;
        cout << "2. View Messages"   << endl;
        cout << "3. Reply to Message"<< endl;
        cout << "4. Delete a Message"<< endl;
        cout << "5. Search Messages" << endl;
        cout << "0. Back"            << endl;
        cout << "Choice: ";
        int choice; cin >> choice;

        // 1. SEND
        if (choice == 1) {
            string content;
            cout << "Enter message: ";
            cin.ignore();
            getline(cin, content);
            Message msg(getCurrentUsername(), content);
            msg.addEmoji(":)");
             msg.addEmoji(":(");
             msg.addEmoji(":D");
             msg.addEmoji("<3");
             msg.addEmoji(":thumbsup:");
            chat->addMessage(msg);
            cout << "[✓] Message sent." << endl;
        }

        // 2. VIEW
        else if (choice == 2) {
            chat->displayChat();
        }

        // 3. REPLY
        else if (choice == 3) {
            if (chat->getMessageCount() == 0) {
                cout << "[!] No messages to reply to." << endl;
                continue;
            }
            chat->displayChat();
            cout << "Enter message index to reply to (0 to "
                 << chat->getMessageCount() - 1 << "): ";
            int idx; cin >> idx;

            Message* original = chat->getMessageAt(idx);
            if (!original) {
                cout << "[!] Invalid index." << endl;
                continue;
            }

            string content;
            cout << "Enter your reply: ";
            cin.ignore();
            getline(cin, content);

            Message reply(getCurrentUsername(), content);
            reply.setReplyTo(original);
            chat->addMessage(reply);
            cout << "[✓] Reply sent." << endl;
        }

        // 4. DELETE
        else if (choice == 4) {
            if (chat->getMessageCount() == 0) {
                cout << "[!] No messages to delete." << endl;
                continue;
            }
            chat->displayChat();
            cout << "Enter message index to delete (0 to "
                 << chat->getMessageCount() - 1 << "): ";
            int idx; cin >> idx;
            chat->deleteMessage(idx, getCurrentUsername());
        }

        // 5. SEARCH
        else if (choice == 5) {
            string keyword;
            cout << "Enter keyword to search: ";
            cin >> keyword;

            vector<Message> results = chat->searchMessages(keyword);
            if (results.empty()) {
                cout << "[!] No messages found containing \""
                     << keyword << "\"." << endl;
            } else {
                cout << "[✓] Found " << results.size()
                     << " message(s):" << endl;
                for (const auto& m : results)
                    m.display();
            }
        }

        else if (choice == 0) break;
        else cout << "[!] Invalid option." << endl;
    }
}

    void sendMessage(Chat* chat)
    {
        string content;
        cout << "Enter message: ";
        cin.ignore();
        getline(cin, content);
        Message msg(getCurrentUsername(), content);
        chat->addMessage(msg);
        cout << "[✓] Message sent." << endl;
    }

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

        for (int i = 0; i < users.size(); i++)
        {
            if (users[i].getPhoneNumber() == phone)
            {
                cout << "This phone number is already linked to an account." << endl;
                return;
            }
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

    void startPrivateChat() {
    string user1 = getCurrentUsername(), user2;
    cout << "Enter the username you want to chat with: ";
    cin >> user2;

    if (findUserIndex(user2) == -1) {
        cout << "User not found." << endl;
        return;
    }

    // Check if chat already exists
    Chat* existingChat = nullptr;
    for (int i = 0; i < (int)chats.size(); i++) {
        PrivateChat* pc = dynamic_cast<PrivateChat*>(chats[i]);
        if (pc && pc->isParticipant(user1) && pc->isParticipant(user2)) {
            existingChat = chats[i];
            break;
        }
    }

    if (!existingChat) {
        existingChat = new PrivateChat(user1, user2);
        chats.push_back(existingChat);
        cout << "Chat started!" << endl;
    } else {
        cout << "Reopening existing chat." << endl;
    }

    openChatSession(existingChat);
}

    void createGroup()
    {
        string chatname;
        vector<string> members;
        string member;
        int flag=0;
        int n;
        cin.ignore();
        cout<<"Enter group name:"<<endl;
        getline(cin,chatname);
        cout<<"Enter number of members in group:";
        cin>>n;


        while(n<=0)
        {
            cout<<"Please enter a valid group number:";
            cin>>n;
        }
        cin.ignore();
        cout<<"Enter the members of group:";
        for(int i=0; i<(n-1); i++)
        {

            getline(cin,member);
            flag=0;

            for(int j=0; j<users.size(); j++)
            {
                if(users[j].getUsername()==member)

                {
                    if(member==getCurrentUsername()){
                        break;
                    }
                    flag=1;
                    break;
                }
            }
            if(flag==1)
            {
                members.push_back(member);
            }
            else
            {
                cout<<"Please enter another member:";
                i--;
            }

        }

        string admin=getCurrentUsername();
        GroupChat* chat=new GroupChat(members,chatname,admin);

        chats.push_back(chat);

        cout<<"Group is created successfully"<<endl;
        chat->displayChat();
        string message;
        cout<<"Please enter your message (or type exit to leave the chat):"<<endl;
        while(true)
        {

            getline(cin, message);
            if(message=="exit")
            {
                break;
            }
            Message msg(getCurrentUsername(),message);
             msg.addEmoji(":)");
             msg.addEmoji(":(");
             msg.addEmoji(":D");
             msg.addEmoji("<3");
             msg.addEmoji(":thumbsup:");
            chat->addMessage(msg);
        }




    }

    void viewChats()
    {
        if (!isLoggedIn())
        {
            cout << " [!] You must be logged in to view your chats." << endl;
            return;
        }

        string currentUser = getCurrentUsername();
        vector<Chat*> myChats;

        for (int i = 0; i < chats.size(); i++)
        {
            bool inChat = false;
            for (int j = 0; j < chats[i]->participants.size(); j++)
            {
                if (chats[i]->participants[j] == currentUser)
                {
                    inChat = true;
                    break;
                }
            }

            if (inChat)
            {
                myChats.push_back(chats[i]);
            }
        }

        if (myChats.empty())
        {
            cout << "\n You have no active chats. Start a private chat or create a group!" << endl;
            return;
        }

        cout << "\n========== YOUR CHATS ==========" << endl;
        for (int i = 0; i < myChats.size(); i++)
        {
            cout << " " << i + 1 << ". " << myChats[i]->chatName << endl;
        }
        cout << "================================" << endl;

        cout << "Enter the number of the chat to open (or 0 to return to main menu): ";
        int choice;
        cin >> choice;

        if (choice == 0)
        {
            return;
        }
        else if (choice > 0 && choice <= myChats.size())
        {
            openChatSession(myChats[choice - 1]);
        }
        else
        {
            cout << " [!] Invalid selection. Returning to menu." << endl;
        }
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

                if (choice == 1)
                    login();
                else if (choice == 2)
                    signUp();
                else if (choice == 3)
                    break;
                else
                    cout << "Invalid choice, try again." << endl;
            }
            else
            {
                cout << "\nHello, " << getCurrentUsername() << "!" << endl;
                cout << "1. Start Private Chat\n2. Create Group\n3. View Chats\n4. Logout\nChoice: ";
                int choice;
                cin >> choice;

                if (choice == 1)
                    startPrivateChat();
                else if (choice == 2)
                    createGroup();
                else if (choice == 3)
                    viewChats();
                else if (choice == 4)
                    logout();
                else
                    cout << "Invalid choice, try again." << endl;
            }
        }
    }
};

// ========================
//          MAIN
// ========================
int main()
{
     SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    WhatsApp whatsapp;
    whatsapp.run();
    return 0;

}
