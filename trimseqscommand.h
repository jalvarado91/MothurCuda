#ifndef TRIMSEQSCOMMAND_H
#define TRIMSEQSCOMMAND_H

/*
 *  trimseqscommand.h
 *  Mothur
 *
 *  Created by Pat Schloss on 6/6/09.
 *  Copyright 2009 Patrick D. Schloss. All rights reserved.
 *
 */

#include "mothur.h"
#include "command.hpp"
#include "sequence.hpp"
#include "qualityscores.h"
#include "groupmap.h"

class TrimSeqsCommand : public Command {
public:
	TrimSeqsCommand(string);
	TrimSeqsCommand();
	~TrimSeqsCommand(){}
	
	vector<string> setParameters();
	string getCommandName()			{ return "trim.seqs";	}
	string getCommandCategory()		{ return "Sequence Processing";		}
	string getHelpString();	
	string getCitation() { return "http://www.mothur.org/wiki/Trim.seqs"; }
	string getDescription()		{ return "provides the preprocessing features needed to screen and sort pyrosequences"; }

	int execute(); 
	void help() { m->mothurOut(getHelpString()); }	
	
private:
	
	GroupMap* groupMap;
	
	struct linePair {
		unsigned long long start;
		unsigned long long end;
		linePair(unsigned long long i, unsigned long long j) : start(i), end(j) {}
	};
	
	bool getOligos(vector<vector<string> >&, vector<vector<string> >&, vector<vector<string> >&);
	bool keepFirstTrim(Sequence&, QualityScores&);
	bool removeLastTrim(Sequence&, QualityScores&);
	bool cullLength(Sequence&);
	bool cullHomoP(Sequence&);
	bool cullAmbigs(Sequence&);

	bool abort, createGroup;
	string fastaFile, oligoFile, qFileName, groupfile, nameFile, outputDir;
	
	bool flip, allFiles, qtrim;
	int numFPrimers, numRPrimers, maxAmbig, maxHomoP, minLength, maxLength, processors, tdiffs, bdiffs, pdiffs, comboStarts;
	int qWindowSize, qWindowStep, keepFirst, removeLast;
	double qRollAverage, qThreshold, qWindowAverage, qAverage;
	vector<string> revPrimer, outputNames;
	set<string> filesToRemove;
	map<string, int> barcodes;
	vector<string> groupVector;
	map<string, int> primers;
	map<string, int> combos;
	map<string, int> groupToIndex;
	vector<string> primerNameVector;	//needed here?
	vector<string> barcodeNameVector;	//needed here?
	map<string, int> groupCounts;  
	map<string, string> nameMap;

	vector<int> processIDS;   //processid
	vector<linePair*> lines;
	vector<linePair*> qLines;
	
	int driverCreateTrim(string, string, string, string, string, string, string, string, string, vector<vector<string> >, vector<vector<string> >, vector<vector<string> >, linePair*, linePair*);	
	int createProcessesCreateTrim(string, string, string, string, string, string, string, string, string, vector<vector<string> >, vector<vector<string> >, vector<vector<string> >);
	int setLines(string, string, vector<unsigned long long>&, vector<unsigned long long>&);
};

#endif
