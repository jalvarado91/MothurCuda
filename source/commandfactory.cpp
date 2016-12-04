/*
 *  commandfactory.cpp
 *
 *
 *  Created by Pat Schloss on 10/25/08.
 *  Copyright 2008 Patrick D. Schloss. All rights reserved.
 *
 */

#include "command.hpp"
#include "clustercommand.h"
#include "collectcommand.h"
#include "collectsharedcommand.h"
#include "getgroupcommand.h"
#include "getlabelcommand.h"
#include "rarefactcommand.h"
#include "summarycommand.h"
#include "summarysharedcommand.h"
#include "rarefactsharedcommand.h"
#include "quitcommand.h"
#include "helpcommand.h"
#include "commandfactory.hpp"
#include "deconvolutecommand.h"
#include "parsimonycommand.h"
#include "unifracunweightedcommand.h"
#include "unifracweightedcommand.h"
#include "libshuffcommand.h"
#include "heatmapcommand.h"
#include "heatmapsimcommand.h"
#include "filterseqscommand.h"
#include "venncommand.h"
#include "nocommands.h"
#include "binsequencecommand.h"
#include "getoturepcommand.h"
#include "treegroupscommand.h"
#include "distancecommand.h"
#include "aligncommand.h"
#include "matrixoutputcommand.h"
#include "getsabundcommand.h"
#include "getrabundcommand.h"
#include "seqsummarycommand.h"
#include "screenseqscommand.h"
#include "reversecommand.h"
#include "trimseqscommand.h"
#include "mergefilecommand.h"
#include "listseqscommand.h"
#include "getseqscommand.h"
#include "removeseqscommand.h"
#include "systemcommand.h"
#include "secondarystructurecommand.h"
#include "getsharedotucommand.h"
#include "getlistcountcommand.h"
#include "classifyseqscommand.h"
#include "phylotypecommand.h"
#include "mgclustercommand.h"
#include "preclustercommand.h"
#include "pcoacommand.h"
#include "otuhierarchycommand.h"
#include "setdircommand.h"
#include "parselistscommand.h"
#include "chimeraccodecommand.h"
#include "chimeracheckcommand.h"
#include "chimeraslayercommand.h"
#include "chimerapintailcommand.h"
#include "chimerabellerophoncommand.h"
#include "chimerauchimecommand.h"
#include "setlogfilecommand.h"
#include "phylodiversitycommand.h"
#include "makegroupcommand.h"
#include "chopseqscommand.h"
#include "clearcutcommand.h"
#include "catchallcommand.h"
#include "splitabundcommand.h"
#include "clustersplitcommand.h"
#include "classifyotucommand.h"
#include "degapseqscommand.h"
#include "getrelabundcommand.h"
#include "sensspeccommand.h"
#include "sffinfocommand.h"
#include "seqerrorcommand.h"
#include "normalizesharedcommand.h"
#include "metastatscommand.h"
#include "splitgroupscommand.h"
#include "clusterfragmentscommand.h"
#include "getlineagecommand.h"
#include "removelineagecommand.h"
#include "parsefastaqcommand.h"
#include "deuniqueseqscommand.h"
#include "pairwiseseqscommand.h"
#include "clusterdoturcommand.h"
#include "subsamplecommand.h"
#include "removegroupscommand.h"
#include "getgroupscommand.h"
#include "indicatorcommand.h"
#include "consensusseqscommand.h"
#include "trimflowscommand.h"
#include "corraxescommand.h"
#include "shhhercommand.h"
#include "pcacommand.h"
#include "nmdscommand.h"
#include "removerarecommand.h"
#include "mergegroupscommand.h"
#include "amovacommand.h"
#include "homovacommand.h"
#include "mantelcommand.h"
#include "makefastqcommand.h"
#include "anosimcommand.h"
#include "getcurrentcommand.h"
#include "setcurrentcommand.h"
#include "sharedcommand.h"
#include "getcommandinfocommand.h"
#include "deuniquetreecommand.h"
#include "countseqscommand.h"
#include "countgroupscommand.h"
#include "summarytaxcommand.h"
#include "chimeraperseuscommand.h"
#include "shhhseqscommand.h"
#include "summaryqualcommand.h"
#include "otuassociationcommand.h"
#include "sortseqscommand.h"
#include "classifytreecommand.h"
#include "cooccurrencecommand.h"
#include "pcrseqscommand.h"
#include "createdatabasecommand.h"
#include "makebiomcommand.h"
#include "getcoremicrobiomecommand.h"
#include "listotulabelscommand.h"
#include "getotulabelscommand.h"
#include "removeotulabelscommand.h"
#include "makecontigscommand.h"
#include "sffmultiplecommand.h"
#include "classifysvmsharedcommand.h"
#include "classifyrfsharedcommand.h"
#include "filtersharedcommand.h"
#include "primerdesigncommand.h"
#include "getdistscommand.h"
#include "removedistscommand.h"
#include "mergetaxsummarycommand.h"
#include "getmetacommunitycommand.h"
#include "sparcccommand.h"
#include "makelookupcommand.h"
#include "renameseqscommand.h"
#include "makelefsecommand.h"
#include "lefsecommand.h"
#include "kruskalwalliscommand.h"
#include "sracommand.h"
#include "mergesfffilecommand.h"
#include "getmimarkspackagecommand.h"
#include "mimarksattributescommand.h"
#include "setseedcommand.h"
#include "makefilecommand.h"
#include "biominfocommand.h"
#include "renamefilecommand.h"
#include "chimeravsearchcommand.h"
#include "mergecountcommand.hpp"

//needed for testing project
//CommandFactory* CommandFactory::_uniqueInstance;

/*******************************************************/

/******************************************************/
CommandFactory* CommandFactory::getInstance() {
	if( _uniqueInstance == 0) {
		_uniqueInstance = new CommandFactory();
	}
	return _uniqueInstance;
}
/***********************************************************/

/***********************************************************/
CommandFactory::CommandFactory(){
	string s = "";
	m = MothurOut::getInstance();

	command = new NoCommand(s);
	shellcommand = new NoCommand(s);
	pipecommand = new NoCommand(s);

	outputDir = ""; inputDir = "";
	logFileName = "";
	append = false;

	//initialize list of valid commands
	commands["make.shared"]			= "make.shared";
	commands["bin.seqs"]			= "bin.seqs";
	commands["get.oturep"]			= "get.oturep";
	commands["cluster"]				= "cluster";
	commands["unique.seqs"]			= "unique.seqs";
	commands["dist.shared"]			= "dist.shared";
	commands["collect.single"]		= "collect.single";
	commands["collect.shared"]		= "collect.shared";
	commands["rarefaction.single"]	= "rarefaction.single";
	commands["rarefaction.shared"]	= "rarefaction.shared";
	commands["summary.single"]		= "summary.single";
	commands["summary.shared"]		= "summary.shared";
	commands["parsimony"]			= "parsimony";
	commands["unifrac.weighted"]	= "unifrac.weighted";
	commands["unifrac.unweighted"]	= "unifrac.unweighted";
	commands["libshuff"]			= "libshuff";
	commands["tree.shared"]			= "tree.shared";
	commands["heatmap.bin"]			= "heatmap.bin";
	commands["heatmap.sim"]			= "heatmap.sim";
	commands["venn"]				= "venn";
	commands["get.group"]           = "get.group";
	commands["get.label"]           = "get.label";
	commands["get.sabund"]          = "get.sabund";
	commands["get.rabund"]          = "get.rabund";
	commands["help"]				= "help";
	commands["reverse.seqs"]		= "reverse.seqs";
	commands["trim.seqs"]			= "trim.seqs";
	commands["trim.flows"]			= "trim.flows";
	commands["list.seqs"]			= "list.seqs";
	commands["get.seqs"]			= "get.seqs";
	commands["remove.seqs"]			= "remove.seqs";
	commands["system"]				= "system";
	commands["align.check"]			= "align.check";
	commands["get.sharedseqs"]		= "get.sharedseqs";
	commands["get.otulist"]			= "get.otulist";
	commands["phylotype"]			= "phylotype";
	commands["mgcluster"]			= "mgcluster";
	commands["pre.cluster"]			= "pre.cluster";
	commands["pcoa"]				= "pcoa";
	commands["otu.hierarchy"]		= "otu.hierarchy";
	commands["set.dir"]				= "set.dir";
	commands["merge.files"]			= "merge.files";
	commands["parse.list"]			= "parse.list";
	commands["set.logfile"]			= "set.logfile";
	commands["phylo.diversity"]		= "phylo.diversity";
	commands["make.group"]			= "make.group";
	commands["chop.seqs"]			= "chop.seqs";
	commands["clearcut"]			= "clearcut";
	commands["catchall"]			= "catchall";
	commands["split.abund"]			= "split.abund";
	commands["classify.otu"]		= "classify.otu";
	commands["degap.seqs"]			= "degap.seqs";
	commands["get.relabund"]		= "get.relabund";
	commands["sffinfo"]				= "sffinfo";
	commands["normalize.shared"]	= "normalize.shared";
	commands["metastats"]			= "metastats";
	commands["split.groups"]		= "split.groups";
	commands["cluster.fragments"]	= "cluster.fragments";
	commands["get.lineage"]			= "get.lineage";
	commands["remove.lineage"]		= "remove.lineage";
	commands["fastq.info"]			= "fastq.info";
	commands["deunique.seqs"]		= "deunique.seqs";
	commands["cluster.classic"]		= "cluster.classic";
	commands["sub.sample"]			= "sub.sample";
	commands["remove.groups"]		= "remove.groups";
	commands["get.groups"]			= "get.groups";
	commands["get.otus"]			= "get.otus";
	commands["remove.otus"]			= "remove.otus";
	commands["indicator"]			= "indicator";
	commands["consensus.seqs"]		= "consensus.seqs";
	commands["corr.axes"]			= "corr.axes";
	commands["pca"]					= "pca";
	commands["nmds"]				= "nmds";
	commands["remove.rare"]			= "remove.rare";
	commands["amova"]				= "amova";
	commands["homova"]				= "homova";
	commands["mantel"]				= "mantel";
	commands["anosim"]				= "anosim";
	commands["make.fastq"]			= "make.fastq";
	commands["merge.groups"]		= "merge.groups";
	commands["get.current"]			= "get.current";
	commands["set.current"]			= "set.current";
	commands["get.commandinfo"]		= "get.commandinfo";
	commands["deunique.tree"]		= "deunique.tree";
	commands["count.seqs"]			= "count.seqs";
	commands["count.groups"]		= "count.groups";
	commands["pairwise.seqs"]		= "pairwise.seqs";
	commands["classify.seqs"]		= "classify.seqs";
	commands["dist.seqs"]			= "dist.seqs";
	commands["filter.seqs"]			= "filter.seqs";
	commands["align.seqs"]			= "align.seqs";
	commands["chimera.ccode"]		= "chimera.ccode";
	commands["chimera.check"]		= "chimera.check";
	commands["chimera.slayer"]		= "chimera.slayer";
	commands["chimera.uchime"]		= "chimera.uchime";
	commands["chimera.perseus"]		= "chimera.perseus";
	commands["chimera.pintail"]		= "chimera.pintail";
	commands["chimera.bellerophon"]	= "chimera.bellerophon";
    commands["chimera.vsearch"]     = "chimera.vsearch";
	commands["screen.seqs"]			= "screen.seqs";
	commands["summary.seqs"]		= "summary.seqs";
	commands["cluster.split"]		= "cluster.split";
	commands["shhh.flows"]			= "shhh.flows";
	commands["sens.spec"]			= "sens.spec";
	commands["seq.error"]			= "seq.error";
	commands["summary.tax"]			= "summary.tax";
	commands["summary.qual"]		= "summary.qual";
	commands["shhh.seqs"]			= "shhh.seqs";
	commands["otu.association"]		= "otu.association";
    commands["sort.seqs"]           = "sort.seqs";
    commands["classify.tree"]       = "classify.tree";
    commands["cooccurrence"]        = "cooccurrence";
    commands["pcr.seqs"]            = "pcr.seqs";
    commands["create.database"]     = "create.database";
    commands["make.biom"]           = "make.biom";
    commands["get.coremicrobiome"]  = "get.coremicrobiome";
    commands["list.otus"]           = "list.otus";
    commands["list.otulabels"]      = "list.otulabels";
    commands["get.otulabels"]       = "get.otulabels";
    commands["remove.otulabels"]    = "remove.otulabels";
    commands["make.contigs"]        = "make.contigs";
    commands["make.table"]          = "make.table";
    commands["sff.multiple"]        = "sff.multiple";
	commands["quit"]				= "quit";
    //commands["classify.rf"]         = "classify.rf";
    commands["classify.svm"]        = "classify.svm";
    commands["filter.shared"]		= "filter.shared";
    commands["primer.design"]		= "primer.design";
    commands["get.dists"]           = "get.dists";
    commands["remove.dists"]        = "remove.dists";
    commands["merge.taxsummary"]    = "merge.taxsummary";
    commands["get.communitytype"]   = "get.communitytype";
    commands["sparcc"]              = "sparcc";
    commands["make.lookup"]         = "make.lookup";
    commands["rename.seqs"]         = "rename.seqs";
    commands["make.lefse"]          = "make.lefse";
    commands["lefse"]               = "lefse";
    commands["kruskal.wallis"]      = "kruskal.wallis";
    commands["make.sra"]            = "make.sra";
    commands["merge.sfffiles"]      = "merge.sfffiles";
    commands["get.mimarkspackage"]  = "get.mimarkspackage";
    commands["mimarks.attributes"]  = "mimarks.attributes";
    commands["make.file"]           = "make.file";
    commands["biom.info"]           = "biom.info";
    commands["set.seed"]            = "set.seed";
    commands["rename.file"]         = "rename.file";
    commands["merge.count"]         = "merge.count";


}

/***********************************************************/
CommandFactory::~CommandFactory(){
	_uniqueInstance = 0;
	delete command;
	delete shellcommand;
	delete pipecommand;
}
/***********************************************************/

/***********************************************************/
int CommandFactory::checkForRedirects(string optionString) {
    try {

        int pos = optionString.find("outputdir");
        if (pos != string::npos) { //user has set outputdir in command option string
            string outputOption = "";
            bool foundEquals = false;
            for(int i=pos;i<optionString.length();i++){
                if(optionString[i] == ',')       { break;               }
                else if(optionString[i] == '=')  { foundEquals = true;	}
                if (foundEquals)       {   outputOption += optionString[i]; }
            }
            if (outputOption[0] == '=') { outputOption = outputOption.substr(1); }
            if(m->mkDir(outputOption)){
                setOutputDirectory(outputOption); 
                m->mothurOut("Setting output directory to: " + outputOption); m->mothurOutEndLine();
            }
        }

        pos = optionString.find("inputdir");
        if (pos != string::npos) { //user has set inputdir in command option string
            string intputOption = "";
            bool foundEquals = false;
            for(int i=pos;i<optionString.length();i++){
                if(optionString[i] == ',')       { break;               }
                else if(optionString[i] == '=')  { foundEquals = true;	}
                if (foundEquals)       {   intputOption += optionString[i]; }
            }
            if (intputOption[0] == '=') { intputOption = intputOption.substr(1); }
            if(m->dirCheck(intputOption)){
                setInputDirectory(intputOption); 
                m->mothurOut("Setting input directory to: " + intputOption); m->mothurOutEndLine();
            }
        }

        pos = optionString.find("seed=");
        if (pos != string::npos) { //user has set seed in command option string
            string intputOption = "";
            bool foundEquals = false;
            for(int i=pos;i<optionString.length();i++){
                if(optionString[i] == ',')       { break;               }
                else if(optionString[i] == '=')  { foundEquals = true;	}
                if (foundEquals)       {   intputOption += optionString[i]; }
            }
            if (intputOption[0] == '=') { intputOption = intputOption.substr(1); }
            bool seed = false; int random;
            if (intputOption == "clear") {
                random = time(NULL);
                seed = true;
            }else {
                if (m->isNumeric1(intputOption)) { m->mothurConvert(intputOption, random); seed=true; }
                else { m->mothurOut("[ERROR]: Seed must be an integer."); m->mothurOutEndLine(); seed = false;}
            }

            if (seed)  {
                srand(random);
                m->mothurOut("Setting random seed to " + toString(random) + ".\n\n");
            }
        }


        return 0;
	}
	catch(exception& e) {
		m->errorOut(e, "CommandFactory", "getCommand");
		exit(1);
	}
}
/***********************************************************/

/***********************************************************/
//This function calls the appropriate command fucntions based on user input.
Command* CommandFactory::getCommand(string commandName, string optionString){
	try {

		delete command;   //delete the old command

        checkForRedirects(optionString);

		//user has opted to redirect output from dir where input files are located to some other place
		if (outputDir != "") {
			if (optionString != "") { optionString += ", outputdir=" + outputDir; }
			else { optionString += "outputdir=" + outputDir; }
		}

		//user has opted to redirect input from dir where mothur.exe is located to some other place
		if (inputDir != "") {
			if (optionString != "") { optionString += ", inputdir=" + inputDir; }
			else { optionString += "inputdir=" + inputDir; }
		}

		if(commandName == "cluster")                    {	command = new ClusterCommand(optionString);					}
		else if(commandName == "quit")					{	command = new QuitCommand(optionString);					}
		else if(commandName == "dist.seqs")				{   command = new DistanceCommand(optionString);				}
		else if(commandName == "pre.cluster")			{	command = new PreClusterCommand(optionString);				}
		else if(commandName == "set.dir")				{	command = new SetDirectoryCommand(optionString);			}
		else if(commandName == "cluster.classic")		{	command = new ClusterDoturCommand(optionString);			}
			else											{	command = new NoCommand(optionString);						}

		return command;
	}
	catch(exception& e) {
		m->errorOut(e, "CommandFactory", "getCommand");
		exit(1);
	}
}
/***********************************************************/
bool CommandFactory::isValidCommand(string command) {
	try {

		//is the command in the map
		if ((commands.find(command)) != (commands.end())) {
			return true;
		}else{
			m->mothurOut(command + " is not a valid command in Mothur.  Valid commands are ");
			for (it = commands.begin(); it != commands.end(); it++) {
				m->mothurOut(it->first + ", ");
			}
			m->mothurOutEndLine();
			return false;
		}

	}
	catch(exception& e) {
		m->errorOut(e, "CommandFactory", "isValidCommand");
		exit(1);
	}
}
/***********************************************************************/
bool CommandFactory::isValidCommand(string command, string noError) {
	try {

		//is the command in the map
		if ((commands.find(command)) != (commands.end())) {
			return true;
		}else{
			return false;
		}

	}
	catch(exception& e) {
		m->errorOut(e, "CommandFactory", "isValidCommand");
		exit(1);
	}
}
/***********************************************************************/
void CommandFactory::printCommands(ostream& out) {
	try {
        it = commands.begin();
		out << "Valid commands are: " << it->first << ", ";
        it++;
        out << it->first;
        it++;
		for (; it != commands.end(); it++) {
			out << ", " << it->first;
		}
		out <<  "." << endl;
	}
	catch(exception& e) {
		m->errorOut(e, "CommandFactory", "printCommands");
		exit(1);
	}
}
/***********************************************************************/
void CommandFactory::printCommandsCategories(ostream& out) {
    try {
        map<string, string> commands = getListCommands();
        map<string, string>::iterator it;

        map<string, string> categories;
        map<string, string>::iterator itCat;
        //loop through each command outputting info
        for (it = commands.begin(); it != commands.end(); it++) {

            Command* thisCommand = getCommand(it->first);

            //don't add hidden commands
            if (thisCommand->getCommandCategory() != "Hidden") {
                itCat = categories.find(thisCommand->getCommandCategory());
                if (itCat == categories.end()) {
                    categories[thisCommand->getCommandCategory()] = thisCommand->getCommandName();
                }else {
                    categories[thisCommand->getCommandCategory()] += ", " + thisCommand->getCommandName();
                }
            }
        }

        for (itCat = categories.begin(); itCat != categories.end(); itCat++) {
            out << itCat->first << " commmands include: " << itCat->second << endl;
        }

    }
	catch(exception& e) {
		m->errorOut(e, "CommandFactory", "printCommandsCategories");
		exit(1);
	}
}

/***********************************************************************/
