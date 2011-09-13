/*
 *  preclustercommand.cpp
 *  Mothur
 *
 *  Created by westcott on 12/21/09.
 *  Copyright 2009 Schloss Lab. All rights reserved.
 *
 */

#include "preclustercommand.h"
#include "sequenceparser.h"
#include "deconvolutecommand.h"

//**********************************************************************************************************************
inline bool comparePriority(seqPNode first, seqPNode second) {  return (first.numIdentical > second.numIdentical); }

//**********************************************************************************************************************
vector<string> PreClusterCommand::setParameters(){	
	try {
		CommandParameter pfasta("fasta", "InputTypes", "", "", "none", "none", "none",false,true); parameters.push_back(pfasta);
		CommandParameter pname("name", "InputTypes", "", "", "none", "none", "none",false,false); parameters.push_back(pname);
		CommandParameter pgroup("group", "InputTypes", "", "", "none", "none", "none",false,false); parameters.push_back(pgroup);
		CommandParameter pdiffs("diffs", "Number", "", "0", "", "", "",false,false); parameters.push_back(pdiffs);
		CommandParameter pbygroup("bygroup", "Boolean", "", "T", "", "", "",false,false); parameters.push_back(pbygroup);
		CommandParameter pinputdir("inputdir", "String", "", "", "", "", "",false,false); parameters.push_back(pinputdir);
		CommandParameter poutputdir("outputdir", "String", "", "", "", "", "",false,false); parameters.push_back(poutputdir);
		
		vector<string> myArray;
		for (int i = 0; i < parameters.size(); i++) {	myArray.push_back(parameters[i].name);		}
		return myArray;
	}
	catch(exception& e) {
		m->errorOut(e, "PreClusterCommand", "setParameters");
		exit(1);
	}
}
//**********************************************************************************************************************
string PreClusterCommand::getHelpString(){	
	try {
		string helpString = "";
		helpString += "The pre.cluster command groups sequences that are within a given number of base mismatches.\n";
		helpString += "The pre.cluster command outputs a new fasta and name file.\n";
		helpString += "The pre.cluster command parameters are fasta, names and diffs. The fasta parameter is required. \n";
		helpString += "The names parameter allows you to give a list of seqs that are identical. This file is 2 columns, first column is name or representative sequence, second column is a list of its identical sequences separated by commas.\n";
		helpString += "The group parameter allows you to provide a group file. \n";
		helpString += "The diffs parameter allows you to specify maximum number of mismatched bases allowed between sequences in a grouping. The default is 1.\n";
		helpString += "The bygroup parameter allows you to indicate you whether you want to cluster sequences only within each group, default=T, when a groupfile is given. \n";
		helpString += "The pre.cluster command should be in the following format: \n";
		helpString += "pre.cluster(fasta=yourFastaFile, names=yourNamesFile, diffs=yourMaxDiffs) \n";
		helpString += "Example pre.cluster(fasta=amazon.fasta, diffs=2).\n";
		helpString += "Note: No spaces between parameter labels (i.e. fasta), '=' and parameters (i.e.yourFasta).\n";
		return helpString;
	}
	catch(exception& e) {
		m->errorOut(e, "PreClusterCommand", "getHelpString");
		exit(1);
	}
}

//**********************************************************************************************************************
PreClusterCommand::PreClusterCommand(){	
	try {
		abort = true; calledHelp = true; 
		setParameters();
		vector<string> tempOutNames;
		outputTypes["fasta"] = tempOutNames;
		outputTypes["name"] = tempOutNames;
	}
	catch(exception& e) {
		m->errorOut(e, "PreClusterCommand", "PreClusterCommand");
		exit(1);
	}
}
//**********************************************************************************************************************

PreClusterCommand::PreClusterCommand(string option) {
	try {
		abort = false; calledHelp = false;   
		
		//allow user to run help
		if(option == "help") { help(); abort = true; calledHelp = true; }
		else if(option == "citation") { citation(); abort = true; calledHelp = true;}
		
		else {
			vector<string> myArray = setParameters();
			
			OptionParser parser(option);
			map<string, string> parameters = parser.getParameters();
			
			ValidParameters validParameter;
			map<string, string>::iterator it;
		
			//check to make sure all parameters are valid for command
			for (map<string, string>::iterator it2 = parameters.begin(); it2 != parameters.end(); it2++) { 
				if (validParameter.isValidParameter(it2->first, myArray, it2->second) != true) {  abort = true;  }
			}
			
			//initialize outputTypes
			vector<string> tempOutNames;
			outputTypes["fasta"] = tempOutNames;
			outputTypes["name"] = tempOutNames;
		
			//if the user changes the input directory command factory will send this info to us in the output parameter 
			string inputDir = validParameter.validFile(parameters, "inputdir", false);		
			if (inputDir == "not found"){	inputDir = "";		}
			else {
				string path;
				it = parameters.find("fasta");
				//user has given a template file
				if(it != parameters.end()){ 
					path = m->hasPath(it->second);
					//if the user has not given a path then, add inputdir. else leave path alone.
					if (path == "") {	parameters["fasta"] = inputDir + it->second;		}
				}
				
				it = parameters.find("name");
				//user has given a template file
				if(it != parameters.end()){ 
					path = m->hasPath(it->second);
					//if the user has not given a path then, add inputdir. else leave path alone.
					if (path == "") {	parameters["name"] = inputDir + it->second;		}
				}
				
				it = parameters.find("group");
				//user has given a template file
				if(it != parameters.end()){ 
					path = m->hasPath(it->second);
					//if the user has not given a path then, add inputdir. else leave path alone.
					if (path == "") {	parameters["group"] = inputDir + it->second;		}
				}
			}

			//check for required parameters
			fastafile = validParameter.validFile(parameters, "fasta", true);
			if (fastafile == "not found") { 				
				fastafile = m->getFastaFile(); 
				if (fastafile != "") { m->mothurOut("Using " + fastafile + " as input file for the fasta parameter."); m->mothurOutEndLine(); }
				else { 	m->mothurOut("You have no current fastafile and the fasta parameter is required."); m->mothurOutEndLine(); abort = true; }
			}
			else if (fastafile == "not open") { abort = true; }	
			else { m->setFastaFile(fastafile); }
			
			//if the user changes the output directory command factory will send this info to us in the output parameter 
			outputDir = validParameter.validFile(parameters, "outputdir", false);		if (outputDir == "not found"){	
				outputDir = "";	
				outputDir += m->hasPath(fastafile); //if user entered a file with a path then preserve it	
			}

			//check for optional parameter and set defaults
			// ...at some point should added some additional type checking...
			namefile = validParameter.validFile(parameters, "name", true);
			if (namefile == "not found") { namefile =  "";  }
			else if (namefile == "not open") { abort = true; }	
			else {  m->setNameFile(namefile); }
			
			groupfile = validParameter.validFile(parameters, "group", true);
			if (groupfile == "not found") { groupfile =  "";  }
			else if (groupfile == "not open") { abort = true; groupfile =  ""; }	
			else {   m->setGroupFile(groupfile); }
			
			string temp	= validParameter.validFile(parameters, "diffs", false);		if(temp == "not found"){	temp = "1"; }
			convert(temp, diffs); 
			
			temp = validParameter.validFile(parameters, "bygroup", false);			
			if (temp == "not found") { 
				if (groupfile != "") { temp = "T"; }
				else { temp = "F"; }
			}
			bygroup = m->isTrue(temp); 
			
			if (bygroup && (groupfile == "")) { m->mothurOut("You cannot set bygroup=T, unless you provide a groupfile."); m->mothurOutEndLine(); abort=true; }

		}
				
	}
	catch(exception& e) {
		m->errorOut(e, "PreClusterCommand", "PreClusterCommand");
		exit(1);
	}
}
//**********************************************************************************************************************

int PreClusterCommand::execute(){
	try {
		
		if (abort == true) { if (calledHelp) { return 0; }  return 2;	}
		
		int start = time(NULL);
		
		string fileroot = outputDir + m->getRootName(m->getSimpleName(fastafile));
		string newFastaFile = fileroot + "precluster" + m->getExtension(fastafile);
		string newNamesFile = fileroot + "precluster.names";
		
		if (bygroup) {
			//clear out old files
			ofstream outFasta; m->openOutputFile(newFastaFile, outFasta); outFasta.close();
			ofstream outNames; m->openOutputFile(newNamesFile, outNames);  outNames.close();
			
			//parse fasta and name file by group
			SequenceParser* parser;
			if (namefile != "") { parser = new SequenceParser(groupfile, fastafile, namefile);	}
			else				{ parser = new SequenceParser(groupfile, fastafile);			}
			
			vector<string> groups = parser->getNamesOfGroups();
			
			//precluster each group
			for (int i = 0; i < groups.size(); i++) {
				
				start = time(NULL);
				
				if (m->control_pressed) {  delete parser; m->mothurRemove(newFastaFile); m->mothurRemove(newNamesFile); return 0; }
				
				m->mothurOutEndLine(); m->mothurOut("Processing group " + groups[i] + ":"); m->mothurOutEndLine();
				
				map<string, string> thisNameMap;
				if (namefile != "") { thisNameMap = parser->getNameMap(groups[i]); }
				vector<Sequence> thisSeqs = parser->getSeqs(groups[i]);
				
				//fill alignSeqs with this groups info.
				int numSeqs = loadSeqs(thisNameMap, thisSeqs);
				
				if (m->control_pressed) {  delete parser; m->mothurRemove(newFastaFile); m->mothurRemove(newNamesFile); return 0; }
				
				if (diffs > length) { m->mothurOut("Error: diffs is greater than your sequence length."); m->mothurOutEndLine(); return 0;  }
				
				int count = process();
				
				if (m->control_pressed) {  delete parser; m->mothurRemove(newFastaFile); m->mothurRemove(newNamesFile); return 0; }

				m->mothurOut("Total number of sequences before precluster was " + toString(alignSeqs.size()) + "."); m->mothurOutEndLine();
				m->mothurOut("pre.cluster removed " + toString(count) + " sequences."); m->mothurOutEndLine(); m->mothurOutEndLine(); 
				printData(newFastaFile, newNamesFile);
				
				m->mothurOut("It took " + toString(time(NULL) - start) + " secs to cluster " + toString(numSeqs) + " sequences."); m->mothurOutEndLine(); 
				
			}
			
			//run unique.seqs for deconvolute results
			string inputString = "fasta=" + newFastaFile;
			if (namefile != "") { inputString += ", name=" + newNamesFile; }
			m->mothurOutEndLine(); 
			m->mothurOut("/******************************************/"); m->mothurOutEndLine(); 
			m->mothurOut("Running command: unique.seqs(" + inputString + ")"); m->mothurOutEndLine(); 
			
			Command* uniqueCommand = new DeconvoluteCommand(inputString);
			uniqueCommand->execute();
			
			map<string, vector<string> > filenames = uniqueCommand->getOutputFiles();
			
			delete uniqueCommand;
			
			m->mothurOut("/******************************************/"); m->mothurOutEndLine(); 
			
			newNamesFile = filenames["name"][0];
			newFastaFile = filenames["fasta"][0];
			
		}else {
			if (namefile != "") { readNameFile(); }
		
			//reads fasta file and return number of seqs
			int numSeqs = readFASTA(); //fills alignSeqs and makes all seqs active
		
			if (m->control_pressed) { return 0; }
	
			if (numSeqs == 0) { m->mothurOut("Error reading fasta file...please correct."); m->mothurOutEndLine(); return 0;  }
			if (diffs > length) { m->mothurOut("Error: diffs is greater than your sequence length."); m->mothurOutEndLine(); return 0;  }
			
			int count = process();
			
			if (m->control_pressed) { return 0; }	
			
			m->mothurOut("Total number of sequences before precluster was " + toString(alignSeqs.size()) + "."); m->mothurOutEndLine();
			m->mothurOut("pre.cluster removed " + toString(count) + " sequences."); m->mothurOutEndLine(); m->mothurOutEndLine(); 
			printData(newFastaFile, newNamesFile);
			
			m->mothurOut("It took " + toString(time(NULL) - start) + " secs to cluster " + toString(numSeqs) + " sequences."); m->mothurOutEndLine(); 
		}
		
		if (m->control_pressed) { m->mothurRemove(newFastaFile); m->mothurRemove(newNamesFile); return 0; }
		
		m->mothurOutEndLine();
		m->mothurOut("Output File Names: "); m->mothurOutEndLine();
		m->mothurOut(newFastaFile); m->mothurOutEndLine();	outputNames.push_back(newFastaFile); outputTypes["fasta"].push_back(newFastaFile);
		m->mothurOut(newNamesFile); m->mothurOutEndLine();	outputNames.push_back(newNamesFile); outputTypes["name"].push_back(newNamesFile);
		m->mothurOutEndLine();
		
		//set fasta file as new current fastafile
		string current = "";
		itTypes = outputTypes.find("fasta");
		if (itTypes != outputTypes.end()) {
			if ((itTypes->second).size() != 0) { current = (itTypes->second)[0]; m->setFastaFile(current); }
		}
		
		itTypes = outputTypes.find("name");
		if (itTypes != outputTypes.end()) {
			if ((itTypes->second).size() != 0) { current = (itTypes->second)[0]; m->setNameFile(current); }
		}
		
		return 0;
		
	}
	catch(exception& e) {
		m->errorOut(e, "PreClusterCommand", "execute");
		exit(1);
	}
}
/**************************************************************************************************/
int PreClusterCommand::process(){
	try {
		
		//sort seqs by number of identical seqs
		sort(alignSeqs.begin(), alignSeqs.end(), comparePriority);
		
		int count = 0;
		int numSeqs = alignSeqs.size();
		
		//think about running through twice...
		for (int i = 0; i < numSeqs; i++) {
			
			//are you active
			//			itActive = active.find(alignSeqs[i].seq.getName());
			
			if (alignSeqs[i].active) {  //this sequence has not been merged yet
				
				//try to merge it with all smaller seqs
				for (int j = i+1; j < numSeqs; j++) {
					
					if (m->control_pressed) { return 0; }
					
					if (alignSeqs[j].active) {  //this sequence has not been merged yet
						//are you within "diff" bases
						int mismatch = calcMisMatches(alignSeqs[i].seq.getAligned(), alignSeqs[j].seq.getAligned());
						
						if (mismatch <= diffs) {
							//merge
							alignSeqs[i].names += ',' + alignSeqs[j].names;
							alignSeqs[i].numIdentical += alignSeqs[j].numIdentical;
							
							alignSeqs[j].active = 0;
							alignSeqs[j].numIdentical = 0;
							count++;
						}
					}//end if j active
				}//end if i != j
				
				//remove from active list 
				alignSeqs[i].active = 0;
				
			}//end if active i
			if(i % 100 == 0)	{ m->mothurOut(toString(i) + "\t" + toString(numSeqs - count) + "\t" + toString(count)); m->mothurOutEndLine();	}
		}
		
		if(numSeqs % 100 != 0)	{ m->mothurOut(toString(numSeqs) + "\t" + toString(numSeqs - count) + "\t" + toString(count)); m->mothurOutEndLine();	}	
		
		return count;
		
	}
	catch(exception& e) {
		m->errorOut(e, "PreClusterCommand", "process");
		exit(1);
	}
}
/**************************************************************************************************/
int PreClusterCommand::readFASTA(){
	try {
		//ifstream inNames;
		ifstream inFasta;
		
		//m->openInputFile(namefile, inNames);
		m->openInputFile(fastafile, inFasta);
		
		//string firstCol, secondCol, nameString;
		length = 0;
		
		while (!inFasta.eof()) {
			
			if (m->control_pressed) { inFasta.close(); return 0; }
			
			//inNames >> firstCol >> secondCol;
			//nameString = secondCol;
			
			//m->gobble(inNames);
			//int size = 1;
			//while (secondCol.find_first_of(',') != -1) { 
			//	size++;
			//	secondCol = secondCol.substr(secondCol.find_first_of(',')+1, secondCol.length());
			//}
			
			Sequence seq(inFasta);  m->gobble(inFasta);
			
			if (seq.getName() != "") {  //can get "" if commented line is at end of fasta file
				if (namefile != "") {
					itSize = sizes.find(seq.getName());
					
					if (itSize == sizes.end()) { m->mothurOut(seq.getName() + " is not in your names file, please correct."); m->mothurOutEndLine(); exit(1); }
					else{
						seqPNode tempNode(itSize->second, seq, names[seq.getName()]);
						alignSeqs.push_back(tempNode);
						if (seq.getAligned().length() > length) {  length = seq.getAligned().length();  }
					}	
				}else { //no names file, you are identical to yourself 
					seqPNode tempNode(1, seq, seq.getName());
					alignSeqs.push_back(tempNode);
					if (seq.getAligned().length() > length) {  length = seq.getAligned().length();  }
				}
			}
		}
		inFasta.close();
		//inNames.close();
		return alignSeqs.size();
	}
	
	catch(exception& e) {
		m->errorOut(e, "PreClusterCommand", "readFASTA");
		exit(1);
	}
}
/**************************************************************************************************/
int PreClusterCommand::loadSeqs(map<string, string>& thisName, vector<Sequence>& thisSeqs){
	try {
		length = 0;
		alignSeqs.clear();
		map<string, string>::iterator it;
		bool error = false;
		 	
		for (int i = 0; i < thisSeqs.size(); i++) {
			
			if (m->control_pressed) { return 0; }
						
			if (namefile != "") {
				it = thisName.find(thisSeqs[i].getName());
				
				//should never be true since parser checks for this
				if (it == thisName.end()) { m->mothurOut(thisSeqs[i].getName() + " is not in your names file, please correct."); m->mothurOutEndLine(); error = true; }
				else{
					//get number of reps
					int numReps = 0;
					for(int j=0;j<(it->second).length();j++){
						if((it->second)[j] == ','){	numReps++;	}
					}
					
					seqPNode tempNode(numReps, thisSeqs[i], it->second);
					alignSeqs.push_back(tempNode);
					if (thisSeqs[i].getAligned().length() > length) {  length = thisSeqs[i].getAligned().length();  }
				}	
			}else { //no names file, you are identical to yourself 
				seqPNode tempNode(1, thisSeqs[i], thisSeqs[i].getName());
				alignSeqs.push_back(tempNode);
				if (thisSeqs[i].getAligned().length() > length) {  length = thisSeqs[i].getAligned().length();  }
			}
		}
		
		//sanity check
		if (error) { m->control_pressed = true; }
		
		thisSeqs.clear();
		
		return alignSeqs.size();
	}
	
	catch(exception& e) {
		m->errorOut(e, "PreClusterCommand", "loadSeqs");
		exit(1);
	}
}
				
/**************************************************************************************************/

int PreClusterCommand::calcMisMatches(string seq1, string seq2){
	try {
		int numBad = 0;
		
		for (int i = 0; i < seq1.length(); i++) {
			//do they match
			if (seq1[i] != seq2[i]) { numBad++; }
			if (numBad > diffs) { return length;  } //to far to cluster
		}
		
		return numBad;
	}
	catch(exception& e) {
		m->errorOut(e, "PreClusterCommand", "calcMisMatches");
		exit(1);
	}
}

/**************************************************************************************************/

void PreClusterCommand::printData(string newfasta, string newname){
	try {
		ofstream outFasta;
		ofstream outNames;
		
		if (bygroup) {
			m->openOutputFileAppend(newfasta, outFasta);
			m->openOutputFileAppend(newname, outNames);
		}else {
			m->openOutputFile(newfasta, outFasta);
			m->openOutputFile(newname, outNames);
		}
				
		for (int i = 0; i < alignSeqs.size(); i++) {
			if (alignSeqs[i].numIdentical != 0) {
				alignSeqs[i].seq.printSequence(outFasta); 
				outNames << alignSeqs[i].seq.getName() << '\t' << alignSeqs[i].names << endl;
			}
		}
		
		outFasta.close();
		outNames.close();
		
	}
	catch(exception& e) {
		m->errorOut(e, "PreClusterCommand", "printData");
		exit(1);
	}
}
/**************************************************************************************************/

void PreClusterCommand::readNameFile(){
	try {
		ifstream in;
		m->openInputFile(namefile, in);
		string firstCol, secondCol;
				
		while (!in.eof()) {
			in >> firstCol >> secondCol; m->gobble(in);
			names[firstCol] = secondCol;
			int size = 1;

			for(int i=0;i<secondCol.size();i++){
				if(secondCol[i] == ','){	size++;	}
			}
			sizes[firstCol] = size;
		}
		in.close();
	}
	catch(exception& e) {
		m->errorOut(e, "PreClusterCommand", "readNameFile");
		exit(1);
	}
}

/**************************************************************************************************/


