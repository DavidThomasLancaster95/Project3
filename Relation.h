#ifndef RELATION_H
#define RELATION_H

#include <string>
#include <set>
#include <vector>
#include "SchemeVec.h"
#include "TupleVec.h"
#include <string>
#include "ConstantMaker.h"

class Relation
{
private:
	std::string nameOfRelation;

	SchemeVec schemeVector;
	
	std::set<TupleVec> setOfTupleVecs;

	


public:
	void setNameOfRelation(std::string inString) {
		nameOfRelation = inString;
	}
	void addToSchemeVector(std::string inString) {
		schemeVector.pushBack(inString);
	}
	void addTupleVectorToSet(TupleVec tupleVec) {
		setOfTupleVecs.insert(tupleVec);
		
	}
	std::string getNameOfRelation() {
		return nameOfRelation;
	}

	SchemeVec getSchemeVector() {
		return schemeVector;
	}
	
	void printRelation() {
		std::cout << "Name:" << nameOfRelation << "\n";
		for (int i = 0; i < schemeVector.size(); i++) {
			std::cout << schemeVector.at(i) << "- ";
			
		}
		std::cout << "\n";

		for (auto it = setOfTupleVecs.begin(); it != setOfTupleVecs.end(); ++it) {
			TupleVec testVec = *it;
			for (int k = 0; k < testVec.size(); k++) {
				std::cout << testVec.at(k) << " ";
			}
			std::cout << "\n";
		}
	}
	Relation SelectString(std::string inString, int index) {
		Relation returnRelation;
		// Set the Name
		returnRelation.setNameOfRelation(nameOfRelation);

		// Add all the parts to the vector
		for (int i = 0; i < schemeVector.size(); i++) {
			returnRelation.addToSchemeVector(schemeVector.at(i));
		}
		// Add the Correct Rows
		for (auto it = setOfTupleVecs.begin(); it != setOfTupleVecs.end(); ++it) {
			TupleVec testVec = *it;
			if (testVec.at(index) == inString) {
				returnRelation.addTupleVectorToSet(testVec);
			}
			
		}
		
		return returnRelation;
	}

	bool integerIsInVectorOfIntegers(std::vector<int> inVec, int index){
		for (unsigned int i = 0; i < inVec.size(); i++) {
			if (inVec.at(i) == index){
				return true;
			}
		}
		return false;
	}
	
	std::set<TupleVec> getSetOfTupleVecs() {
		return setOfTupleVecs;
	}

	Relation SelectConstant(ConstantMaker constantMaker) {
		Relation returnRelation;
		// Set the Name
		returnRelation.setNameOfRelation(nameOfRelation);

		// Add all the parts to the vector
		for (int i = 0; i < schemeVector.size(); i++) {
			 
			// This part below does the renaming.... Could be changed to use the rename algorithm. 
			if (/* i is a constant marker*/integerIsInVectorOfIntegers(constantMaker.getPositionVector(), i)) {
				// add the constantMaker.name to return relation
				returnRelation.addToSchemeVector(constantMaker.getConstantValue());
			}
			else {
				returnRelation.addToSchemeVector(schemeVector.at(i));
			}
			

			//returnRelation.addToSchemeVector(schemeVector.at(i));
		}

		// Add the Correct Rows
		for (auto it = setOfTupleVecs.begin(); it != setOfTupleVecs.end(); ++it) {
			TupleVec testVec = *it;
			
			if (elementsAreEqual(constantMaker, testVec)) {
				returnRelation.addTupleVectorToSet(testVec);
			}

		}

		return returnRelation;
	}

	void Rename(std::string inName, std::string nameToChange){
		for (int i = 0; i < schemeVector.size(); i++) {
			if (schemeVector.at(i) == nameToChange) {
				schemeVector.at(i) == inName;
			}
		}
	}

	void StripAndRename(ConstantMaker constantMaker){
		Project(constantMaker.getPositionVector());
	}

	void PrintRelationTuples() {

		//Id='33333', A='12 Apple St.', P='555-1234'
		
		

		for (auto it = setOfTupleVecs.begin(); it != setOfTupleVecs.end(); ++it) {
			TupleVec testVec = *it;

			if (testVec.size() > 0) {
				std::cout << "  ";
				for (int k = 0; k < testVec.size(); k++) {

					if (k == testVec.size() - 1) {
						std::cout << schemeVector.at(k) << "=";
						std::cout << testVec.at(k);

					}
					else {
						std::cout << schemeVector.at(k) << "=";
						std::cout << testVec.at(k) << ", ";
					}



				}
				if (setOfTupleVecs.size() > 1) {
					//std::cout << "\n";
				}
				std::cout << "\n";
			}

			
			
		}
	}

	void Project(std::vector<int> inputVector){
		for (int i = schemeVector.size() - 1; 0 < i; i--) {
			if (!(std::count(inputVector.begin(), inputVector.end(), i))) {
				schemeVector.at(i+1).erase(schemeVector.at(i+1).begin() + i+1);
				std::cout << "Removed\n";
			}
		}


		// TODO finish this
	}


	void ProjectByIndex(int index) {
		
		for (int i = schemeVector.size() - 1; i >= 0; i--) {
			if (i == index) {

				schemeVector.changeVal(i, "FAG");
				if (schemeVector.at(i) == "FAG") {
					schemeVector.removeElementByValue("FAG");
				}
			}
		}

		// now to remove all the tuples

		std::set<TupleVec> replacementSet;


		for (auto it = setOfTupleVecs.begin(); it != setOfTupleVecs.end(); ++it) {
			TupleVec testVec = *it;

			TupleVec replacementTupleVec;

			for (int k = 0; k < testVec.size(); k++) {
				if (k != index) {
					replacementTupleVec.pushBack(testVec.at(k));
				}
			}

			replacementSet.insert(replacementTupleVec);
		}
		setOfTupleVecs = replacementSet;
	}

	void ProjectByString(std::string removeString) {
		int removeIndex = 0;

		for (int i = schemeVector.size() - 1; i >= 0 ; i--) {
			if (schemeVector.at(i) == removeString) {
				
				removeIndex = i;
				schemeVector.changeVal(i, "FAG");
				if (schemeVector.at(i) == "FAG") {
					schemeVector.removeElementByValue("FAG");
				}
			}
		}

		// now to remove all the tuples

		std::set<TupleVec> replacementSet;


		for (auto it = setOfTupleVecs.begin(); it != setOfTupleVecs.end(); ++it) {
			TupleVec testVec = *it;

			TupleVec replacementTupleVec;

			for (int k = 0; k < testVec.size(); k++) {
				if (k != removeIndex) {
					replacementTupleVec.pushBack(testVec.at(k));
				}
			}
			//std::cout << removeIndex << " " << testVec.at(removeIndex) << "\n";


			replacementSet.insert(replacementTupleVec);
			



			//testVec.removeElementByIndex(removeIndex);
			
		}

		setOfTupleVecs = replacementSet;

	}

	bool elementsAreEqual(ConstantMaker constantMaker, TupleVec tupleVec) {
		
		std::string commonValue = tupleVec.at(constantMaker.getPositionVector().at(0));

		for (unsigned int i = 0; i < constantMaker.getPositionVector().size(); i++) {
			if (tupleVec.getVector().at(constantMaker.getPositionVector().at(i)) == commonValue) {

			}
			else {
				return false; 
			}
		}

		return true; 

	}

	
	
	

};

#endif