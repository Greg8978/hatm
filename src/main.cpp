#include <iostream>

#include <sound_play/sound_play.h>
#include <msgClient.hh>
#include <hatpPlan.hh>


#include <ros/ros.h>
#include "ros/package.h"
#include <queue>
#include "htn_verbalizer2/Empty.h"
#include "htn_verbalizer2/NodeParam.h"
#include "htn_verbalizer2/Name.h"
#include "htn_verbalizer2/Names.h"

#include "toaster_msgs/GetFactValue.h"
#include "toaster_msgs/AddFact.h"
#include "toaster_msgs/Fact.h"

#include <tinyxml.h>

std::string clientName_ = "hatptester"; //Name should actually be the same as Michelangelo supervisor !!!
msgClient hatpClient_;
hatpPlan* plan_;
sound_play::SoundClient* soundClient_;
ros::ServiceClient* getKnowledgeClient_;
ros::ServiceClient* setKnowledgeClient_;
unsigned int nbPartners_ = 0; // This is use so that robot will say "you" if 1 partner and tell name if more than 1

// Tinyxml
TiXmlDocument listNodes_;
TiXmlDocument listNames_;
TiXmlDocument listKnowledges_;
TiXmlElement *knowledges_;
TiXmlElement *nodes_;
TiXmlElement *names_;


//Agents in plan
std::vector<std::string> agents_;

// Speakin agent
// TODO: put it in a service
std::string speakingAgent_ = " HERAKLES_HUMAN1";

//To ask robot to repeat:
std::string lastSentence_ = "";

// The user want the robot to repeat
bool repeat_ = false;

// Policy for robot to explain his task or not
bool policyTeach_ = false;

void verbalize(std::string sentence, unsigned int time) {
    soundClient_->say(sentence);
    printf("[saying] %s\n", sentence.c_str());
    sleep(time);
    lastSentence_ = sentence;
}

/**
 * returns the class of a parameter
 * @param param parameter from which we want the class
 * @return 
 */
std::string getParameterClass(std::string param) {
    if (param == "Green_Cube" || param == "Blue_Cube" || param == "Red_Cube")
        return "Cubes";
    else if (param == "Glue_Green" || param == "Glue_Red" || param == "Glue_Blue")
        return "Glue";
    else
        return param;
}

/**
 * 
 * @param plan
 * @return 
 */
std::string planNamesToSpeech(std::string plan) {

    TiXmlElement *current_name = names_;

    while (current_name) //for each element of the xml file
    {
        if (current_name->Attribute("name") == plan) { //if it is the good one
            return current_name->Attribute("speech");
        } else
            current_name = current_name->NextSiblingElement();
    }

    return plan;
}

/**
 * 
 * @param id
 * @return 
 */
std::string nodeToText(unsigned int id) {
    std::stringstream ss;

    TiXmlElement *current_node = nodes_;

    while (current_node) //for each element of the xml file
    {
        if (current_node->Attribute("node") == plan_->getNode(id)->getName()) { //if it is the good one
            ss << current_node->Attribute("speech1");

            if (current_node->Attribute("arg1"))
                ss << planNamesToSpeech(plan_->getNode(id)->getParameters()[atoi(current_node->Attribute("arg1"))]);

            ss << current_node->Attribute("speech2");

            if (current_node->Attribute("arg2")) {
                ss << planNamesToSpeech(plan_->getNode(id)->getParameters()[atoi(current_node->Attribute("arg2"))]);
            }

            return ss.str();
        } else
            current_node = current_node->NextSiblingElement();
    }

    ss << planNamesToSpeech(plan_->getNode(id)->getName());
    return ss.str();
}

unsigned int getTaskFromSpeech(std::string speech, std::vector<unsigned int> currentNodes) {
    for (std::vector<unsigned int>::iterator it = currentNodes.begin(); it != currentNodes.end(); ++it) {
        if (speech.find(nodeToText((*it))) != std::string::npos)
            return (*it);
    }
    return 0;
}

/**
 * 
 * @param id
 * @return 
 */
std::string planToKnowledgeParam(unsigned int id) {

    TiXmlElement *current_knowledge = knowledges_;
    std::stringstream ss;
    while (current_knowledge) //for each element of the xml file
    {
        if (current_knowledge->Attribute("node") == plan_->getNode(id)->getName()) { //if it is the good one
            if (current_knowledge->Attribute("global_knowledge"))
                ss << "global_knowledge";
            else {
                if (current_knowledge->Attribute("arg1")) {
                    if (current_knowledge->Attribute("class1"))
                        ss << getParameterClass(plan_->getNode(id)->getParameters()[atoi(current_knowledge->Attribute("arg1"))]) << "-";
                    else
                        ss << plan_->getNode(id)->getParameters()[atoi(current_knowledge->Attribute("arg1"))] << "-";
                }
                if (current_knowledge->Attribute("arg2")) {
                    if (current_knowledge->Attribute("class2"))
                        ss << getParameterClass(plan_->getNode(id)->getParameters()[atoi(current_knowledge->Attribute("arg2"))]) << "-";
                    else
                        ss << plan_->getNode(id)->getParameters()[atoi(current_knowledge->Attribute("arg2"))] << "-";
                }
            }
            return ss.str();
        } else {
            current_knowledge = current_knowledge->NextSiblingElement();
        }
    }

    // We couldn't find this node. Set default knowledge representation method
    for (int i = 0; i < plan_->getNode(id)->getParameters().size(); ++i) {
        if ((plan_->getNode(id)->getParameters()[i]) == "HERAKLES_HUMAN1" || (plan_->getNode(id)->getParameters()[i]) == "PR2_ROBOT" && i < 2)
            continue;
        else
            ss << plan_->getNode(id)->getParameters()[i] << "-";
    }
    return ss.str();
}

bool askUnderstand() {
    std::string mystr;
    verbalize("Did you understand?", 2);
    getline(std::cin, mystr);
    if (mystr == "yes")
        return true;
    else if (mystr == "no")
        return false;
    else
        return askUnderstand();
    return false;
}

bool inputYesNo() {
    std::string mystr;
    getline(std::cin, mystr);
    if (mystr == "yes")
        return true;
    else if (mystr == "no")
        return false;
    else {
        verbalize("I didn't understand, please answer with yes or no?", 5);
        return inputYesNo();
    }
    return false;
}

bool askRepartitionOk() {
    verbalize("Are you ok with this plan?", 2);
    return inputYesNo();
}

// These are basic functions, should be improved!

bool wantToPerform(std::string mystr) {
    if (mystr.find("i want to") != std::string::npos)
        return true;
    else return false;
}

bool dontWantToPerform(std::string mystr) {
    if (mystr.find("i don't want to") != std::string::npos)
        return true;
    else return false;
}

bool cantPerform(std::string mystr) {
    if (mystr.find("i can't") != std::string::npos)
        return true;
    else return false;
}

bool addWishToDB(unsigned int nodeId, bool add) {
    hatpNode* node = plan_->getNode(nodeId);
    bool success = true;
    std::string params = planToKnowledgeParam(nodeId);



    toaster_msgs::AddFact setPreference;
    setPreference.request.fact.property = node->getName();
    setPreference.request.fact.propertyType = "state";
    setPreference.request.fact.subProperty = "preference";
    setPreference.request.fact.subjectId = speakingAgent_;
    setPreference.request.fact.targetId = params;
    if (add)
        setPreference.request.fact.stringValue = "true";
    else
        setPreference.request.fact.stringValue = "false";

    if (setKnowledgeClient_->call(setPreference)) {
        ROS_INFO("[Request] we request to set preference in PR2_ROBOT model: %s %s %s \n", speakingAgent_.c_str(), params.c_str(), setPreference.request.fact.stringValue.c_str());
    } else {
        ROS_INFO("[Request] we failed to request to set preference in PR2_ROBOT model: %s %s %s \n", speakingAgent_.c_str(), params.c_str(), setPreference.request.fact.stringValue.c_str());
        success = false;
    }


    return success;
}

bool addCantToDB(unsigned int nodeId) {
    hatpNode* node = plan_->getNode(nodeId);
    bool success = true;
    std::string params = planToKnowledgeParam(nodeId);

    toaster_msgs::AddFact setPreference;
    setPreference.request.fact.property = node->getName();
    setPreference.request.fact.propertyType = "state";
    setPreference.request.fact.subProperty = "ability";
    setPreference.request.fact.subjectId = speakingAgent_;
    setPreference.request.fact.targetId = params;
    setPreference.request.fact.stringValue = "false";

    if (setKnowledgeClient_->call(setPreference)) {
        ROS_INFO("[Request] we request to set unability in PR2_ROBOT model: %s %s %s \n", speakingAgent_.c_str(), params.c_str(), setPreference.request.fact.stringValue.c_str());
    } else {
        ROS_INFO("[Request] we failed to request to set unability in PR2_ROBOT model: %s %s %s \n", speakingAgent_.c_str(), params.c_str(), setPreference.request.fact.stringValue.c_str());
        success = false;
    }

    return success;
}

bool userInput(std::vector<unsigned int> currentNodes) {
    std::string mystr;
    //TODO: replace this with speech
    getline(std::cin, mystr);

    if (mystr == "cancel")
        return false;

    unsigned int node = getTaskFromSpeech(mystr, currentNodes);
    if (node == 0) {
        verbalize("I didn't understand, can you repeat please?", 4);
        userInput(currentNodes);
        return true;
    }

    if (wantToPerform(mystr))
        addWishToDB(node, true);
    else if (dontWantToPerform(mystr))
        addWishToDB(node, false);
    else if (cantPerform(mystr))
        addCantToDB(node);

    return true;
}

bool askChangeRepartition(std::vector<unsigned int> currentNodes) {
    verbalize("What is wrong?", 2);

    if (userInput(currentNodes)) {
        verbalize("Got it! Something else?", 4);

        if (inputYesNo())
            askChangeRepartition(currentNodes);

        return true;

    } else
        return false;
}

bool askExplanation() {
    std::string mystr;
    verbalize("Do you need explanation for this task", 3);
    return inputYesNo();
}

/**
 * 
 * @param agents
 * @return 
 */
std::string getSubject(std::vector<std::string> agents) {
    if (agents.size() < 2)
        if (agents[0] == "PR2_ROBOT")
            return "I ";
        else
            return "You ";
    else if (std::find(agents.begin(), agents.end(), "PR2_ROBOT") != agents.end())
        return "We ";
    else
        return "You ";
}

void removePr2(std::vector<std::string>& agents) {
    std::vector<std::string>::iterator it = std::find(agents.begin(), agents.end(), "PR2_ROBOT");
    if (it != agents.end()) {
        agents.erase(it);
    }
}

// We decide here:
// -> to return the lower knowledge value if it concerns several agents

/**
 * 
 * @param id
 * @return 
 */
std::string getKnowledge(unsigned int id) {
    //TODO: implement this function!
    hatpNode* node = plan_->getNode(id);
    std::vector<std::string> agents = node->getAgents();

    std::string curKnowledge = "beginner";
    std::string params = planToKnowledgeParam(id);
    if (params == "global_knowledge")
        return "expert";

    if (agents.size() == 1) {
        if (agents[0] == "PR2_ROBOT") {
            agents = agents_;
            removePr2(agents);
        }
    } else {
        removePr2(agents);
    }

    for (std::vector<std::string>::iterator it = agents.begin(); it != agents.end(); ++it) {

        toaster_msgs::GetFactValue getKnowledge;
        getKnowledge.request.agentId = "pr2";
        getKnowledge.request.reqFact.property = node->getName();
        getKnowledge.request.reqFact.subjectId = (*it);
        getKnowledge.request.reqFact.targetId = params;

        ROS_INFO("[Request] we request knowledge in PR2_ROBOT model: %s %s %s \n", (*it).c_str(), node->getName().c_str(), params.c_str());

        if (getKnowledgeClient_->call(getKnowledge)) {
            // If this agent has less knowledge, we keep his level
            curKnowledge = getKnowledge.response.resFact.stringValue;

            ROS_INFO("[Request] we found: \"%s\" \n", curKnowledge.c_str());

            if (curKnowledge == "")
                return "beginner";
        }
    }
    ROS_INFO("[Request] we got knowledge in PR2_ROBOT model: %s \n", curKnowledge.c_str());
    return curKnowledge;
}

/**
 * 
 * @param level
 * @param nodeId
 * @return 
 */
bool updateKnowledge(std::string level, unsigned int nodeId) {
    hatpNode* node = plan_->getNode(nodeId);
    std::vector<std::string> agents = node->getAgents();
    bool success = true;
    std::string params = planToKnowledgeParam(nodeId);
    if (params == "global_knowledge")
        return true;

    if (agents.size() == 1) {
        if (agents[0] == "PR2_ROBOT") {
            agents = agents_;
        }
    }

    for (std::vector<std::string>::iterator it = agents.begin(); it != agents.end(); ++it) {

        if ((*it) == "PR2_ROBOT")
            continue;

        toaster_msgs::AddFact setKnowledge;
        setKnowledge.request.fact.property = node->getName();
        setKnowledge.request.fact.propertyType = "knowledge";
        setKnowledge.request.fact.subProperty = "action";
        setKnowledge.request.fact.subjectId = (*it);
        setKnowledge.request.fact.targetId = params;
        setKnowledge.request.fact.stringValue = level;

        if (setKnowledgeClient_->call(setKnowledge)) {
            ROS_INFO("[Request] we request to set knowledge in PR2_ROBOT model: %s %s %s \n", (*it).c_str(), params.c_str(), level.c_str());
        } else {
            ROS_INFO("[Request] we failed to request to set knowledge in PR2_ROBOT model: %s %s %s \n", (*it).c_str(), params.c_str(), level.c_str());
            success = false;
        }

    }
    return success;
}

/**
 * 
 * @param agents
 * @param task
 */
void tellTask(std::vector<std::string> agents, unsigned int task) {
    std::stringstream ss;

    ss << getSubject(agents) << "have to  " << nodeToText(task);
    verbalize(ss.str(), 5);
}

/**
 * 
 * @param currentNodes
 * @param daddy
 * @param stillNeeded
 * @return 
 */
std::vector<unsigned int> processNodesOnce(std::vector<unsigned int> currentNodes, unsigned int daddy, bool& stillNeeded) {
    std::vector<unsigned int> processedNodes;

    for (std::vector<unsigned int>::iterator it = currentNodes.begin(); it != currentNodes.end(); ++it) {
        std::vector<unsigned int> children = plan_->getNode((*it))->getSubNodes();

        // If 1 child, verbalize child instead!
        if (children.size() == 1) {
            processedNodes.push_back(children[0]);
            stillNeeded = true;
            continue;

            // If recursive method, verbalize children instead!
        } else if (plan_->getNode((*it))->getName() == plan_->getNode(daddy)->getName()
                && plan_->getNode((*it))->getParameters() == plan_->getNode(daddy)->getParameters()) {
            for (std::vector<unsigned int>::iterator itChildren = children.begin(); itChildren != children.end(); ++itChildren)
                processedNodes.push_back((*itChildren));
            stillNeeded = true;
            continue;
        } else {
            processedNodes.push_back((*it));
        }
    }
    return processedNodes;
}

/**
 * 
 * @param currentNodes
 * @param daddy
 * @return 
 */
std::vector<unsigned int> processNodes(std::vector<unsigned int> currentNodes, unsigned int daddy) {
    std::vector<unsigned int> processedNodes;
    bool processStillNeeded = false;

    processedNodes = processNodesOnce(currentNodes, daddy, processStillNeeded);

    while (processStillNeeded) {
        processStillNeeded = false;
        processedNodes = processNodesOnce(processedNodes, daddy, processStillNeeded);
    }
    return processedNodes;
}

/**
 * 
 * @param currentNodes
 * @param daddy
 */
void verbalizeTasksRepartition(std::vector<unsigned int> currentNodes, unsigned int daddy) {
    //Introduction:

    if (!currentNodes.empty()) {
        std::stringstream ss;
        ss << "I will tell you the steps";
        verbalize(ss.str(), 3);

        for (std::vector<unsigned int>::iterator it = currentNodes.begin(); it != currentNodes.end(); ++it) {
            ss.str("");
            //printf("verbalizing nodes, current %d\n", (*it));

            if (it == currentNodes.begin()) {
                ss << getSubject(plan_->getNode((*it))->getAgents()) << "will first "
                        << nodeToText((*it));

                verbalize(ss.str(), 3);

            } else if (((*it) != currentNodes.back()) || ((*it) == currentNodes[1])) {
                ss << "Then " << getSubject(plan_->getNode((*it))->getAgents()) << "will "
                        << nodeToText((*it));

                verbalize(ss.str(), 3);

            } else {
                ss << "Finally " << getSubject(plan_->getNode((*it))->getAgents()) << "will "
                        << nodeToText((*it));

                verbalize(ss.str(), 3);

            }
        }

        //All children nodes were told, update db for plan repartition:
    }
}

bool executeTree(std::vector<unsigned int> currentNodes, unsigned int daddy) {
    std::stringstream ss;
    std::vector<std::string> agents = plan_->getNode(daddy)->getAgents();
    ss << "Let's start to " << nodeToText(daddy);
    verbalize(ss.str(), 5);
    bool tellcurrent = true;


    //if (getKnowledge(daddy) != "new" || !policyTeach_)
    //   ROS_INFO("knowledge %s   policy %d", getKnowledge(daddy).c_str(), policyTeach_);

    // If only robot is concerned + no need explain
    if ((plan_->getNode(daddy)->getAgents().size() < 2 && agents[0] == "PR2_ROBOT")
            && (getKnowledge(daddy) != "new" || !policyTeach_))
        tellcurrent = false;

    for (std::vector<unsigned int>::iterator it = currentNodes.begin(); it != currentNodes.end(); ++it) {
        ss.str("");
        //printf("verbalizing nodes, current %d\n", (*it));

        if (tellcurrent) {
            if (it == currentNodes.begin()) {

                ss << getSubject(plan_->getNode((*it))->getAgents()) << " first "
                        << nodeToText((*it));

            } else
                ss << "Good, now " << getSubject(plan_->getNode((*it))->getAgents()) << nodeToText((*it));

            //bool understand = false;

            //while (!understand) {
            verbalize(ss.str(), 5);
            //    understand = askUnderstand();
            //}

        }


        agents = plan_->getNode((*it))->getAgents();
        std::vector<unsigned int> children = processNodes(plan_->getNode((*it))->getSubNodes(), (*it));


        ///////////////// No human involved case //////////////
        if (agents.size() < 2 && agents[0] == "PR2_ROBOT") { // only ROBOT case
            // if not leaf, and we need to teach
            if (!children.empty() && getKnowledge((*it)) == "new"
                    && policyTeach_) {
                executeTree(children, (*it));
                updateKnowledge("beginner", (*it));

                // if leaf or no explain needed
            } else {
                //TODO SUPERVISOR execute(current)
                ROS_INFO("SUPERVISION execute(%s)", plan_->getNode((*it))->getName().c_str());
            }

            ////////////// Human involved case /////////////////////
        } else { //Human present case

            // NEW user
            if (getKnowledge((*it)) == "new") {
                //TODO explain((*it));

                ROS_INFO("Explain %s", plan_->getNode((*it))->getName().c_str());

                //If not leaf
                if (!children.empty()) {
                    executeTree(children, (*it));
                } else {
                    ROS_INFO("SUPERVISION monitorAtomic(%s)", plan_->getNode((*it))->getName().c_str());
                    //TODO SUPERVISION monitorAtomic((*it))
                }
                // This should be done only to new user
                // TODO: a way to tell increment all users / only new users
                updateKnowledge("beginner", (*it));


                // BEGINNER user
            } else if (getKnowledge((*it)) == "beginner") {
                bool explain = askExplanation();
                if (explain) {
                    //We downgrade the knowledge
                    updateKnowledge("new", (*it));
                    //TODO explain((*it));
                    ROS_INFO("Explain %s", plan_->getNode((*it))->getName().c_str());
                    //If leaf
                    if (!children.empty()) {
                        executeTree(children, (*it));
                    } else {
                        //TODO SUPERVISION monitorAtomic((*it))
                        ROS_INFO("SUPERVISION monitorAtomic(%s)", plan_->getNode((*it))->getName().c_str());
                    }
                    updateKnowledge("beginner", (*it));
                } else {
                    //TODO SUPERVISION monitorAtomic((*it))
                    ROS_INFO("SUPERVISION monitorAtomic(%s)", plan_->getNode((*it))->getName().c_str());
                    updateKnowledge("intermediate", (*it));
                }


                // INTERMEDIATE user
            } else if (getKnowledge((*it)) == "intermediate") {
                //monitor((*it));
                ROS_INFO("SUPERVISION monitor(%s)", plan_->getNode((*it))->getName().c_str());
                updateKnowledge("expert", (*it));


                // EXPERT user
            } else if (getKnowledge((*it)) == "expert") {
                //monitor((*it));
                ROS_INFO("SUPERVISION monitorAtomic(%s)", plan_->getNode((*it))->getName().c_str());

            }

        }
    }
    // This should be just an update
    updateKnowledge("beginner", daddy);

}

/**
 * 
 * @param n
 * @return 
 */
bool executePlan(unsigned int n) {
    if (plan_ == NULL)
        return false;
    else {
        bool negotiate = false;
        std::vector<std::string> agents = plan_->getNode(n)->getAgents();

        tellTask(agents, n);

        printf("Task verbalized\n");

        if (!plan_->getNode(n)->getSubNodes().empty()) {

            //pre processing
            vector<unsigned int> nodesRemaining = processNodes(plan_->getNode(n)->getSubNodes(), n);
            vector<unsigned int> currentNodes;

            while (!nodesRemaining.empty()) {
                // Human can remember 7 info: 1 goal + 3 task + 3 agent responsible for each task
                // SO we limit the plan presentation to 3 nodes
                if (nodesRemaining.size() > 3) {
                    vector<unsigned int>::const_iterator start = nodesRemaining.begin();
                    vector<unsigned int>::const_iterator cut = nodesRemaining.begin() + 2;
                    vector<unsigned int>::const_iterator end = nodesRemaining.end();

                    currentNodes.assign(start, cut);
                    cut++;
                    nodesRemaining.assign(cut, end);

                } else {
                    currentNodes = nodesRemaining;
                    nodesRemaining.clear();
                }


                verbalizeTasksRepartition(currentNodes, n);
                negotiate = !askRepartitionOk();
                if (negotiate) {
                    if (askChangeRepartition(currentNodes)) {
                        verbalize("Ok, I will try to find a new plan", 5);
                        return false;
                    }
                    // User canceled change
                    verbalize("Ok, I will keep this plan", 5);
                }
                //Execute tree
                executeTree(currentNodes, n);

            }
        }
        printf("tree done\n");
        return true;
    }
}

/**
 * 
 * @param req
 * @param res
 * @return 
 */
bool initPlan(htn_verbalizer2::Empty::Request &req,
        htn_verbalizer2::Empty::Response & res) {

    ROS_INFO("[htn_verbalizer][initPlan] Waiting for a plan");

    std::string answer;

    if (hatpClient_.isConnected()) {
        std::pair<std::string, std::string> result = hatpClient_.getBlockingMessage();
        //std::cout << "#### Answer : \n" << result.second << std::endl;
        answer = result.second;
    } else {
        ROS_INFO("[htn_verbalizer][WARNING] client not connected!");
        return false;
    }

    ROS_INFO("[htn_verbalizer][initPlan] plan received");

    removeFormatting(answer);
    if (testInputValidity(answer)) {
        plan_ = new hatpPlan(answer);
        agents_ = plan_->getTree()->getRootNode()->getAgents();
    } else
        ROS_INFO("[htn_verbalizer][WARNING] unvalid plan received!");

    return true;
}

/**
 * 
 * @param req
 * @param res
 * @return 
 */
bool initSpeech(htn_verbalizer2::Empty::Request &req,
        htn_verbalizer2::Empty::Response & res) {

    if (plan_ != NULL) {
        std::vector<std::string> agents;
        std::stringstream ss;

        agents = plan_->getTree()->getRootNode()->getAgents();

        for (std::vector<std::string>::iterator it = agents.begin(); it != agents.end(); ++it) {
            if ((*it) != "PR2_ROBOT")
                ss << "Hello " << planNamesToSpeech((*it)) << "! ";
        }

        printf("[saying] %s\n", ss.str().c_str());
        soundClient_->say(ss.str());
        sleep(2);
        lastSentence_ = ss.str();


        return true;
    } else {
        ROS_INFO("[htn_verbalizer][initSpeech][WARNING] no plan, use init_plan request!");
        //soundClient_->say("Hello, I will compute a plan for us to complete the task!");
        //sleep(2);
        return false;
    }
}

/**
 * 
 * @param req
 * @param res
 * @return 
 */
bool rePlanActionFailure(htn_verbalizer2::NodeParam::Request &req,
        htn_verbalizer2::NodeParam::Response & res) {

    ROS_INFO("[htn_verbalizer][rePlan] received a replan request!");
    //We start by informing about the error:
    if (plan_ != NULL) {
        std::vector<std::string> agents;
        std::stringstream ss;
        std::string answer;


        agents = plan_->getNode(req.node)->getAgents();

        ss << "Something went wrong: ";
        for (std::vector<std::string>::iterator it = agents.begin(); it != agents.end(); ++it) {


            ss << getSubject(plan_->getNode(req.node)->getAgents()) << " failed to " << planNamesToSpeech(plan_->getNode(req.node)->getName())
                    << " ! Let me think about a new way to " << planNamesToSpeech(plan_->getTree()->getRootNode()->getName());
        }

        verbalize(ss.str(), 5);
        ROS_INFO("[htn_verbalizer][rePlan] Waiting for a plan");


        if (hatpClient_.isConnected()) {
            std::pair<std::string, std::string> result = hatpClient_.getBlockingMessage();
            //std::cout << "#### Answer : \n" << result.second << std::endl;
            answer = result.second;
        } else {
            ROS_INFO("[htn_verbalizer][WARNING] client not connected!");
            return false;
        }

        ROS_INFO("[htn_verbalizer][rePlan] plan received");

        removeFormatting(answer);
        if (testInputValidity(answer)) {
            delete plan_;
            plan_ = new hatpPlan(answer);

            ss.str("");

            ss << " I found a way to " << planNamesToSpeech(plan_->getTree()->getRootNode()->getName())
                    << " from the current situation ";

            verbalize(ss.str(), 5);

            executePlan(plan_->getTree()->getRootNode()->getID());

        } else
            ROS_INFO("[htn_verbalizer][WARNING] unvalid plan received!");

        return true;
    } else {
        ROS_INFO("[htn_verbalizer][initSpeech][WARNING] no plan, use init_plan request!");
        //soundClient_->say("Hello, I will compute a plan for us to complete the task!");
        //sleep(2);
        return false;
    }
}

/**
 * 
 * @param req
 * @param res
 * @return 
 */
bool initExecution(htn_verbalizer2::NodeParam::Request &req,
        htn_verbalizer2::NodeParam::Response & res) {

    ROS_INFO("[htn_verbalizer][initExec] received a initExec request!");
    if (plan_ == NULL) {
        ROS_INFO("[htn_verbalizer][initExec][WARNING] no plan, use init_plan request!");
        return false;
    } else {

        std::stringstream ss;

        ss << getSubject(plan_->getNode(req.node)->getAgents()) << " will now " << planNamesToSpeech(plan_->getNode(req.node)->getName());
        soundClient_->say(ss.str());
        sleep(3);
        printf("[saying] %s\n", ss.str().c_str());
        lastSentence_ = ss.str();
    }
    return true;
}

/**
 * 
 * @param req
 * @param res
 * @return 
 */
bool endExecution(htn_verbalizer2::NodeParam::Request &req,
        htn_verbalizer2::NodeParam::Response & res) {
    std::stringstream ss;

    ROS_INFO("[htn_verbalizer][initExec] received a initExec request!");
    if (plan_ == NULL) {
        ROS_INFO("[htn_verbalizer][initExec][WARNING] no plan, use init_plan request!");
        return false;
    } else {

        std::stringstream ss;
        if (plan_->getNode(req.node)->getAgents().front() != "PR2_ROBOT")
            ss << " I see that ";

        ss << getSubject(plan_->getNode(req.node)->getAgents()) << " finished to " << planNamesToSpeech(plan_->getNode(req.node)->getName());

        verbalize(ss.str(), 5);
    }
    return true;
}

/**
 * 
 * @param req
 * @param res
 * @return 
 */
bool executeCurrentPlan(htn_verbalizer2::Empty::Request &req,
        htn_verbalizer2::Empty::Response & res) {

    if (plan_ == NULL)
        ROS_INFO("[htn_verbalizer][verbalizeCurrentPlan][WARNING] no plan, use init_plan request!");
    else {
        return executePlan(plan_->getTree()->getRootNode()->getID());
    }
    return true;
}

/**
 * 
 * @param req
 * @param res
 * @return 
 */
bool setHatpClient(htn_verbalizer2::Name::Request &req,
        htn_verbalizer2::Name::Response & res) {

    if (req.name == "")
        ROS_INFO("[htn_verbalizer][setHatpPlan][WARNING] no client name given!");
    else {
        clientName_ = req.name;
    }
    return true;
}

bool changePolicyTeach(htn_verbalizer2::Empty::Request &req,
        htn_verbalizer2::Empty::Response & res) {

    if (policyTeach_)
        policyTeach_ = false;
    else
        policyTeach_ = true;

    ROS_INFO("[htn_verbalizer][policyteach] policyTeach_ set to %d", policyTeach_);
    return true;
}

bool setAgentsPresent(htn_verbalizer2::Names::Request &req,
        htn_verbalizer2::Names::Response & res) {

    if (req.names.empty())
        ROS_INFO("[htn_verbalizer][setHatpPlan][WARNING] no agent names given!");
    else {
        agents_ = req.names;
    }
    return true;
}

/**
 * 
 * @param argc
 * @param argv
 * @return 
 */
int main(int argc, char ** argv) {


    ros::init(argc, argv, "htn_verbalizer");

    ros::NodeHandle node;

    sound_play::SoundClient soundClient;
    soundClient_ = &soundClient;
    // Init HATP client
    hatpClient_.connect(clientName_, "localhost", 5500);


    ros::ServiceClient getKnowledgeClient = node.serviceClient<toaster_msgs::GetFactValue>("/belief_manager/get_fact_value", true);
    getKnowledgeClient_ = &getKnowledgeClient;

    ros::ServiceClient setKnowledgeClient = node.serviceClient<toaster_msgs::AddFact>("/belief_manager/add_fact", true);
    setKnowledgeClient_ = &setKnowledgeClient;


    //Services
    ros::ServiceServer serviceInitPlan = node.advertiseService("htn_verbalizer/init_plan", initPlan);
    ROS_INFO("[Request] Ready to receive a plan.");

    ros::ServiceServer serviceInitSpeech = node.advertiseService("htn_verbalizer/init_speech", initSpeech);
    ROS_INFO("[Request] Ready to init speech.");

    ros::ServiceServer serviceVerbCurrent = node.advertiseService("htn_verbalizer/verbalize_current_plan", executeCurrentPlan);
    ROS_INFO("[Request] Ready to verbalize current plan.");

    ros::ServiceServer serviceReplan = node.advertiseService("htn_verbalizer/replan", rePlanActionFailure);
    ROS_INFO("[Request] Ready to replan.");

    ros::ServiceServer serviceInitExe = node.advertiseService("htn_verbalizer/init_execution", initExecution);
    ROS_INFO("[Request] Ready to init execution.");

    ros::ServiceServer serviceEndExec = node.advertiseService("htn_verbalizer/end_execution", endExecution);
    ROS_INFO("[Request] Ready to end execution.");

    ros::ServiceServer servicesetHatp = node.advertiseService("htn_verbalizer/set_hatp_client", setHatpClient);
    ROS_INFO("[Request] Ready to set hatp client.");

    ros::ServiceServer servicePolicy = node.advertiseService("htn_verbalizer/switch_policy", changePolicyTeach);
    ROS_INFO("[Request] Ready to set policy.");

    ros::ServiceServer serviceAg = node.advertiseService("htn_verbalizer/set_present_agents", setAgentsPresent);
    ROS_INFO("[Request] Ready to set present agents.");


    // Set this in a ros service?
    ros::Rate loop_rate(30);

    //open xml files

    // nodes
    std::stringstream pathNodes;
    pathNodes << ros::package::getPath("htn_verbalizer2") << "/data/list_nodes.xml";
    listNodes_ = TiXmlDocument(pathNodes.str());

    if (!listNodes_.LoadFile()) {
        ROS_WARN_ONCE("Error while loading xml file");
        ROS_WARN_ONCE("error # %d", listNodes_.ErrorId());
        ROS_WARN_ONCE("%s", listNodes_.ErrorDesc());
    }

    TiXmlHandle hdl(&listNodes_);
    nodes_ = hdl.FirstChildElement().FirstChildElement().Element();



    // names
    std::stringstream pathNames;
    pathNames << ros::package::getPath("htn_verbalizer2") << "/data/list_names.xml";
    listNames_ = TiXmlDocument(pathNames.str());

    if (!listNames_.LoadFile()) {
        ROS_WARN_ONCE("Error while loading xml file");
        ROS_WARN_ONCE("error # %d", listNames_.ErrorId());
        ROS_WARN_ONCE("%s", listNames_.ErrorDesc());
    }

    TiXmlHandle hdlNames(&listNames_);
    names_ = hdlNames.FirstChildElement().FirstChildElement().Element();

    // knowledges
    std::stringstream pathKnowledges;
    pathKnowledges << ros::package::getPath("htn_verbalizer2") << "/data/list_knowledge.xml";
    listKnowledges_ = TiXmlDocument(pathKnowledges.str());

    if (!listKnowledges_.LoadFile()) {
        ROS_WARN_ONCE("Error while loading xml file");
        ROS_WARN_ONCE("error # %d", listKnowledges_.ErrorId());
        ROS_WARN_ONCE("%s", listKnowledges_.ErrorDesc());
    }

    TiXmlHandle hdlKnowledges(&listKnowledges_);
    knowledges_ = hdlKnowledges.FirstChildElement().FirstChildElement().Element();

    while (node.ok()) {
        ros::spinOnce();
        loop_rate.sleep();
    }

    return 0;
}
