/*
 *  helpcommand.cpp
 *  Dotur
 *
 *  Created by Sarah Westcott on 1/2/09.
 *  Copyright 2009 Schloss Lab UMASS Amherst. All rights reserved.
 *
 */

#include "helpcommand.h"
#include "clustercommand.h"
#include "clusterdoturcommand.h"
#include "command.hpp"
#include "commandfactory.hpp"
#include "distancecommand.h"
#include "helpcommand.h"
#include "nocommands.h"
#include "preclustercommand.h"
#include "quitcommand.h"
#include "setdircommand.h"



//**********************************************************************************************************************

HelpCommand::HelpCommand(string option)  {	
	validCommands = CommandFactory::getInstance();
    
    abort = false; calledHelp = false;
    
    //allow user to run help
    if(option == "help") { help(); abort = true; calledHelp = true; }
    else if(option == "citation") { citation(); abort = true; calledHelp = true;}
    
    commandName = option;
    
}
//**********************************************************************************************************************
int HelpCommand::execute(){
	try {
        if (commandName != "") {
            if (validCommands->isValidCommand(commandName)) {
                Command* command;
                string optionString = "help";
                
                if(commandName == "cluster")                    {   command = new ClusterCommand(optionString);                 }
                else if(commandName == "quit")                  {   command = new QuitCommand(optionString);                    }
                else if(commandName == "dist.seqs")             {   command = new DistanceCommand(optionString);                }
                else if(commandName == "help")                  {   command = new HelpCommand(optionString);                    }
                else if(commandName == "pre.cluster")           {   command = new PreClusterCommand(optionString);              }
                else if(commandName == "set.dir")               {   command = new SetDirectoryCommand(optionString);            }
                else if(commandName == "cluster.classic")       {   command = new ClusterDoturCommand(optionString);            }
                else                                            {   command = new NoCommand(optionString);                      }					}
                
                command->execute();
                delete command;
                
            }else {
                m->mothurOut("[ERROR]: " + commandName + " is not a valid command."); m->mothurOutEndLine();
                validCommands->printCommands(cout);
            }
        }else {
            validCommands->printCommands(cout);
            m->mothurOut("For more information about a specific command type 'commandName(help)' i.e. 'cluster(help)'"); m->mothurOutEndLine();
        }
		
        m->mothurOutEndLine(); m->mothurOut("For further assistance please refer to the Mothur manual on our wiki at http://www.mothur.org/wiki, or contact Pat Schloss at mothur.bugs@gmail.com.\n");
	
		return 0;
	}
	catch(exception& e) {
		m->errorOut(e, "HelpCommand", "execute");
		exit(1);
	}
}
//**********************************************************************************************************************/
