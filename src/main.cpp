#include <iostream>

#include <sound_play/sound_play.h>
#include <msgClient.hh>
#include <hatpPlan.hh>


#include <ros/ros.h>
#include <queue>
#include "htn_verbalizer2/Empty.h"
#include "htn_verbalizer2/NodeParam.h"
#include "htn_verbalizer2/Name.h"
#include "htn_verbalizer2/Names.h"

#include "toaster_msgs/GetFactValue.h"
#include "toaster_msgs/AddFact.h"
#include "toaster_msgs/Fact.h"


std::string clientName_ = "hatptester"; //Name should actually be the same as Michelangelo supervisor !!!
msgClient hatpClient_;
hatpPlan* plan_;
sound_play::SoundClient* soundClient_;
ros::ServiceClient* getKnowledgeClient_;
ros::ServiceClient* setKnowledgeClient_;
unsigned int nbPartners_ = 0; // This is use so that robot will say "you" if 1 partner and tell name if more than 1

//Agents in plan
std::vector<std::string> agents_;

//To ask robot to repeat:
std::string lastSentence_ = "";

// Policy for robot to explain his task or not
bool policyTeach_ = false;

void verbalize(std::string sentence, unsigned int time) {
    soundClient_->say(sentence);
    sleep(time);
    lastSentence_ = sentence;
    printf("[saying] %s\n", sentence.c_str());
}

/**
 * returns the class of a parameter
 * @param param parameter from which we want the class
 * @return 
 */
std::string getParameterClass(std::string param) {
    if ("Green_Cube" || "Blue_Cube" || "Red_Cube")
        return "Cubes";
    else
        return param;
}

/**
 * 
 * @param id
 * @return 
 */
std::string planToKnowledgeParam(unsigned int id) {
    std::stringstream ss;
    ss << "";
    // Some knowledge apply to class of objects, other on object itself

    for (int i = 0; i < plan_->getNode(id)->getParameters().size(); ++i) {
        if ((plan_->getNode(id)->getParameters()[i]) == "HERAKLES_HUMAN1")
            continue;
        if (plan_->getNode(id)->getName() == "PlaceOnStack" || plan_->getNode(id)->getName() == "Pick") {
            if (getParameterClass(plan_->getNode(id)->getParameters()[i]) == "Cubes") {
                ss << getParameterClass(plan_->getNode(id)->getParameters()[i]) << "-";
            } else {
                ss << plan_->getNode(id)->getParameters()[i] << "-";
            }
        } else {
            ss << plan_->getNode(id)->getParameters()[i] << "-";
        }
    }
    return ss.str();
}

/**
 * 
 * @param plan
 * @return 
 */
std::string planNamesToSpeech(std::string plan) {

    if (plan == "Blue_Cube")
        return "blue cube ";
    else if (plan == "Red_Cube")
        return "red cube ";
    else if (plan == "Green_Cube")
        return "green cube ";
    else if (plan == "Stickers")
        return "put stickers on ";
    else if (plan == "ApplyOperation")
        return "process ";
    else
        return plan;
}

/**
 * 
 * @param id
 * @return 
 */
std::string nodeToText(unsigned int id) {
    std::stringstream ss;

    if (plan_->getNode(id)->getName() == "Handle")
        ss << "handle the " << planNamesToSpeech(plan_->getNode(id)->getParameters()[1]);
    else if (plan_->getNode(id)->getName() == "Place")
        ss << "place the " << planNamesToSpeech(plan_->getNode(id)->getParameters()[1]) << " on the " << planNamesToSpeech(plan_->getNode(id)->getParameters()[2]);
    else if (plan_->getNode(id)->getName() == "PlaceOnStack")
        ss << "place the " << planNamesToSpeech(plan_->getNode(id)->getParameters()[1]) << " on the stack ";
    else if (plan_->getNode(id)->getName() == "Pick")
        ss << "pick the " << planNamesToSpeech(plan_->getNode(id)->getParameters()[1]);
    else if (plan_->getNode(id)->getName() == "Apply")
        ss << planNamesToSpeech(plan_->getNode(id)->getParameters()[2]) << " the " << planNamesToSpeech(plan_->getNode(id)->getParameters()[1]);
    else if (plan_->getNode(id)->getName() == "ApplyOperation")
        ss << planNamesToSpeech(plan_->getNode(id)->getName()) << " the " << planNamesToSpeech(plan_->getNode(id)->getParameters()[1]);
    else if (plan_->getNode(id)->getName() == "HandleOperation")
        ss << planNamesToSpeech(plan_->getNode(id)->getParameters()[2]) << " the " << planNamesToSpeech(plan_->getNode(id)->getParameters()[1]);
    else if (plan_->getNode(id)->getName() == "ApplyFirstOperations")
        ss << "Apply operations" << " on the " << planNamesToSpeech(plan_->getNode(id)->getParameters()[1]);
    else
        ss << planNamesToSpeech(plan_->getNode(id)->getName());
    return ss.str();
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

// We decide here:
// -> to return 1.0 if it concerns only PR2_ROBOT
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

    if (agents.size() == 1) {
        if (agents[0] == "PR2_ROBOT") {
            agents = agents_;
        }
    } else {
        std::vector<std::string>::iterator it = std::find(agents.begin(), agents.end(), "PR2_ROBOT");
        if (it != agents.end()) {
            agents.erase(it);
        }
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
void tellTask(std::vector<std::string> agents, std::string task) {
    std::stringstream ss;

    ss << getSubject(agents) << "have to " << planNamesToSpeech(task);
    soundClient_->say(ss.str());
    sleep(3);
    lastSentence_ = ss.str();
    printf("[saying] %s\n", ss.str().c_str());
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
        ss << "I will tell you the steps to " << nodeToText(daddy);
        verbalize(ss.str(), 5);

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
    bool tellWhat = true;


    if(getKnowledge(daddy) != "new" || !policyTeach_)
        ROS_INFO("knowledge %s   policy %d", getKnowledge(daddy).c_str(), policyTeach_);
    
    // If only robot is concerned + no need explain
    if ((plan_->getNode(daddy)->getAgents().size() < 2 && agents[0] == "PR2_ROBOT")
            && (getKnowledge(daddy) != "new" || !policyTeach_))
        tellWhat = false;

    for (std::vector<unsigned int>::iterator it = currentNodes.begin(); it != currentNodes.end(); ++it) {
        ss.str("");
        //printf("verbalizing nodes, current %d\n", (*it));

        if (tellWhat) {
            if (it == currentNodes.begin()) {

                ss << getSubject(plan_->getNode((*it))->getAgents()) << " first "
                        << nodeToText((*it));

            } else
                ss << "Good, now " << getSubject(plan_->getNode((*it))->getAgents()) << nodeToText((*it));

            verbalize(ss.str(), 3);
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
            }

            ////////////// Human involved case /////////////////////
        } else { //Human present case

            // NEW user
            if (getKnowledge((*it)) == "new") {
                //TODO explain((*it));

                //If leaf
                if (!children.empty()) {
                    executeTree(children, (*it));
                } else {
                    //TODO SUPERVISION monitorAtomic((*it))
                }
                // This should be done only to new user
                // TODO: a way to tell increment all users / only new users
                updateKnowledge("beginner", (*it));


                // BEGINNER user
            } else if (getKnowledge((*it)) == "beginner") {
                verbalize("Do you need explanation for this task?", 3);
                // TODO: get real answere
                bool yes = true;
                if (yes) {
                    //We downgrade the knowledge
                    updateKnowledge("new", (*it));
                    //TODO explain((*it));
                    //If leaf
                    if (!children.empty()) {
                        executeTree(children, (*it));
                    } else {
                        //TODO SUPERVISION monitorAtomic((*it))
                    }
                    updateKnowledge("beginner", (*it));
                } else {
                    //TODO SUPERVISION monitorAtomic((*it))
                    updateKnowledge("intermediate", (*it));
                }


                // INTERMEDIATE user
            } else if (getKnowledge((*it)) == "intermediate") {
                //monitor((*it));
                updateKnowledge("expert", (*it));


                // EXPERT user
            } else if (getKnowledge((*it)) == "expert") {
                //monitor((*it));

            }

        }
    }
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
        std::vector<std::string> agents = plan_->getNode(n)->getAgents();
        std::string task = plan_->getNode(n)->getName();

        tellTask(agents, task);

        printf("Task verbalized\n");

        if (!plan_->getNode(n)->getSubNodes().empty()) {

            //pre processing
            vector<unsigned int> nodesRemaining = processNodes(plan_->getNode(n)->getSubNodes(), n);
            vector<unsigned int> currentNodes;

            printf("1 nodes Remaining size? %d\n", (int) nodesRemaining.size());
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

                    printf("2 nodes Remaining size? %d\n", nodesRemaining.size());
                } else {
                    currentNodes = nodesRemaining;
                    nodesRemaining.clear();
                }
                verbalizeTasksRepartition(currentNodes, n);
                //Execute tree
                executeTree(currentNodes, n);

                printf("3 nodes Remaining size? %d\n", nodesRemaining.size());
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
bool rePlan(htn_verbalizer2::NodeParam::Request &req,
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
        executePlan(plan_->getTree()->getRootNode()->getID());
    }

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

}

bool changePolicyTeach(htn_verbalizer2::Empty::Request &req,
        htn_verbalizer2::Empty::Response & res) {

    if (policyTeach_)
        policyTeach_ = false;
    else
        policyTeach_ = true;

    ROS_INFO("[htn_verbalizer][policyteach] policyTeach_ set to %d", policyTeach_);
}

bool setAgentsPresent(htn_verbalizer2::Names::Request &req,
        htn_verbalizer2::Names::Response & res) {

    if (req.names.empty())
        ROS_INFO("[htn_verbalizer][setHatpPlan][WARNING] no agent names given!");
    else {
        agents_ = req.names;
    }

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

    ros::ServiceServer serviceReplan = node.advertiseService("htn_verbalizer/replan", rePlan);
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


    while (node.ok()) {
        ros::spinOnce();
        loop_rate.sleep();
    }

    return 0;
}
