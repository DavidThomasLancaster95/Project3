#ifndef DATABASE_H
#define DATABASE_H

#include "DatalogProgram.h"
#include <vector>
#include "Relation.h"
#include <string>
#include "SchemeVec.h"
#include "ConstantMaker.h"

class DataBase
{
private:

	std::vector<Relation> RelationsVector;
	
	DatalogProgram rawData;

	std::vector<Relation> QueryResultsVector;

public:

	DataBase(DatalogProgram InRawData) {
		//std::cout << "STARTED!\n";
		rawData = InRawData;
		buildRelationsVector();
		processQueries();
		//printAllQueryResults();
		projectRemove();
		printAndProcess();

		

		//std::cout << "FINISHED!\n";
	}

	void buildRelationsVector() {
		// For every element of SchemesPredicate vector...
		for (unsigned int i = 0; i < rawData.getSchemesPredicateVector().size(); i++) {

			// CREATE A NEW RELATION
			Relation relation;

			// ADD A NEW NAME
			relation.setNameOfRelation(rawData.getSchemesPredicateVector().at(i).returnID());

			// ADD every Element of the Scheme Vector
			for (unsigned int k = 0; k < rawData.getSchemesPredicateVector().at(i).returnElements().size(); k++) {
				
				relation.addToSchemeVector(rawData.getSchemesPredicateVector().at(i).returnElements().at(k));
				
			}

			// make a tuple vector
			for (unsigned int k = 0; k < rawData.getFactsPredicateVector().size(); k++) {
				TupleVec myTupleVec;
				
				if (rawData.getFactsPredicateVector().at(k).returnID() == relation.getNameOfRelation()) {
					for (unsigned int j = 0; j < rawData.getFactsPredicateVector().at(k).returnElements().size(); j++) {
						myTupleVec.pushBack(rawData.getFactsPredicateVector().at(k).returnElements().at(j));
						
					}
					relation.addTupleVectorToSet(myTupleVec);

				}
			}
			RelationsVector.push_back(relation);
		}
	}

	void processQueries() {
		for (unsigned int i = 0; i < rawData.getQueriesPredicateVector().size(); i++) {
			processSingleQuery(rawData.getQueriesPredicateVector().at(i));
		}
	}

	void processSingleQuery(Predicate inPredicate) {
		
		for (unsigned int i = 0; i < RelationsVector.size(); i++) {


			if (RelationsVector.at(i).getNameOfRelation() == inPredicate.returnID()) { // match the name
				Relation returnRelation = RelationsVector.at(i);
				// for each parameter...
				
 				for (unsigned int k = 0; k < inPredicate.returnElements().size(); k++) {
					if (/*Its a string */isAString(inPredicate.returnElements().at(k))) {
						returnRelation = returnRelation.SelectString(inPredicate.returnElements().at(k), k);
					}
					else {
						// it its a consonant
						
						ConstantMaker constantMaker(inPredicate.returnElements().at(k));
							// Find and add the values
						//constantMaker.pushPositionVector(k);
						for (unsigned int j = k; j < inPredicate.returnElements().size(); j++) {
							if (inPredicate.returnElements().at(j) == inPredicate.returnElements().at(k)) {
								constantMaker.pushPositionVector(j);
							}
						}

						for (unsigned int x = 0; x < constantMaker.getPositionVector().size(); x++) {
							//std::cout << constantMaker.getPositionVector().at(x) << " ";
						}

						//std::cout << "-n";
						// select for the constant marker. 
						returnRelation = returnRelation.SelectConstant(constantMaker);
						
					}

					
				}

				//int x = 1; // what is the return relation value????

				QueryResultsVector.push_back(returnRelation);
			}


		}
	}

	void projectRemove() {
		// Rename the appropriate schemes
		
		//printAllQueryResults();
		// remove the rows.
		for (int i = QueryResultsVector.size() - 1; i >= 0 ; i--) { // try iterating backwards on this one too original code: unsigned int i = 0; i < QueryResultsVector.size(); i++
			for (int k = QueryResultsVector.at(i).getSchemeVector().size() - 1; k >= 0 ; k--) { // Try iterating backwards original code: unsigned int k = 0; k < QueryResultsVector.at(i).getSchemeVector().size(); k++
				
				std::string asdf = rawData.getQueriesPredicateVector().at(i).returnElements().at(k);
				if (isAString(asdf)) {
					//std::cout << QueryResultsVector.at(i).getSchemeVector().at(k) << "\n";

					QueryResultsVector.at(i).ProjectByString(QueryResultsVector.at(i).getSchemeVector().at(k));
					
				}
				else {
					//removedDuplicateConstants();
				}
			}

			//std::cout << "\n";
		}

		//printAllQueryResults();

		removedDuplicateConstants();

	}
	
	void removedDuplicateConstants() {
		for (int i = QueryResultsVector.size() - 1; i >= 0; i--) { 
			for (int k = QueryResultsVector.at(i).getSchemeVector().size() - 1; k >= 0; k--) { 
				std::string value = QueryResultsVector.at(i).getSchemeVector().at(k);  //rawData.getQueriesPredicateVector().at(i).returnElements().at(k);
				if (!isAString(value)) {
					std::string currentValue = value;
					for (int j = k - 1; j >= 0; j--) {
						std::string value2 = QueryResultsVector.at(i).getSchemeVector().at(j);
						if (value2 == currentValue) {

							//QueryResultsVector.at(i).ProjectByString(currentValue);

							QueryResultsVector.at(i).ProjectByIndex(k);
						}
					}
				}
			}
		}
	}

	void printAndProcess() {
		for (unsigned int i = 0; i < QueryResultsVector.size(); i++) {
			std::cout << rawData.getQueriesPredicateVector().at(i).returnID();
			std::cout << "(";
			for (unsigned int k = 0; k < rawData.getQueriesPredicateVector().at(i).returnElements().size(); k++) {
				if (k == rawData.getQueriesPredicateVector().at(i).returnElements().size() - 1) {
					std::cout << rawData.getQueriesPredicateVector().at(i).returnElements().at(k);
				}
				else {
					std::cout << rawData.getQueriesPredicateVector().at(i).returnElements().at(k) << ",";
				}
			}
			std::cout << ")? ";

			// Yes/no (x) 

			if (QueryResultsVector.at(i).getSetOfTupleVecs().size() > 0) {
				std::cout << "Yes(" << QueryResultsVector.at(i).getSetOfTupleVecs().size() << ")";
			}
			else {
				std::cout << "No";
			}

			std::cout << "\n";

			


			//for (unsigned int k = 0; k < rawData.getQueriesPredicateVector().at(i).returnElements().size(); k++) {
			//	
			//	std::cout << rawData.getQueriesPredicateVector().at(i).returnElements().at(k) << "\n";
			//	if (!isAString(rawData.getQueriesPredicateVector().at(i).returnElements().at(k))) {
			//		// Project that element by string. 
			//		//QueryResultsVector.at(i).
			//		std::cout << "___" << rawData.getQueriesPredicateVector().at(i).returnElements().at(k) << "___\n";
			//		//std::cout << "---" << QueryResultsVector.at(i).getSetOfTupleVecs(). << "---\n";
			//		QueryResultsVector.at(i).ProjectByString(rawData.getQueriesPredicateVector().at(i).returnElements().at(k));
			//	}
			//}

			//QueryResultsVector.at(i).printRelation();
			QueryResultsVector.at(i).PrintRelationTuples();
		}

	}
	


	// -----------------Helper Functions---------------------

	bool isAString(std::string inString) {
		char firstChar = inString.at(0);
		if (firstChar == '\'') {
			return true;
		}
		else {
			return false;
		}
	}

	void printAllQueryResults() {
		std::cout << "\n\n----PRINTING QUERY TESTS----\n\n";
		

		for (unsigned int i = 0; i < QueryResultsVector.size(); i++) {
			QueryResultsVector.at(i).printRelation();


			QueryResultsVector.at(i).PrintRelationTuples();
			std::cout << "\n\n";
		}
	}

};

#endif