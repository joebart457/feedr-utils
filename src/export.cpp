
#include <Rcpp.h>
#include <algorithm>

// [[Rcpp::export]]
Rcpp::DataFrame displacements(Rcpp::StringVector uniqueIds, Rcpp::DatetimeVector timeStamps, int marginInSeconds = 301) {

	Rcpp::StringVector results;

	if (uniqueIds.size() != timeStamps.size())
		throw Rcpp::exception("vector sizes differ");

	std::multimap<double, Rcpp::String> lookup;
	
	for (unsigned int i{ 0 }; i < timeStamps.size(); i++) {
		lookup.insert(std::pair<double,Rcpp::String>(timeStamps[i], uniqueIds[i]));
	}

	for (auto it = lookup.begin(); it != lookup.end(); ++it) {
		Rcpp::Datetime date = it->first;
		if (++it == lookup.end()) {
			results.push_back(NA_STRING);
			break;
		}
		Rcpp::Datetime testAgainst = it->first;
		if (date + marginInSeconds <= testAgainst)
			results.push_back(it->second);
		else results.push_back(NA_STRING);
		--it;
	}
	
	return Rcpp::DataFrame::create(
		Rcpp::Named("Displacee", uniqueIds),
		Rcpp::Named("Displacer", results)
	);
}

// [[Rcpp::export]]
Rcpp::DataFrame encounters(Rcpp::StringVector uniqueIds, Rcpp::DatetimeVector timeStamps, int marginInSeconds = 301) {
	
	Rcpp::StringVector results;
	
	if (uniqueIds.size() != timeStamps.size())
		throw Rcpp::exception("vector sizes differ");
	
	for (unsigned int i{0}; i < timeStamps.size(); i++) {
		Rcpp::Datetime testTarget = timeStamps[i];
		Rcpp::Datetime testTargetUpper = timeStamps[i] + marginInSeconds;
		Rcpp::Datetime testTargetLower = timeStamps[i] - marginInSeconds;

		auto found = std::find_if(timeStamps.begin(), timeStamps.end(), [&testTarget, &testTargetUpper,&testTargetLower,&marginInSeconds](const Rcpp::Datetime& val){
			return val != testTarget && val <= testTargetUpper && val >= testTargetLower;
		});

		auto foundVal = NA_STRING;
		if (found != timeStamps.end()) {
			auto index = found - timeStamps.begin();
			foundVal = uniqueIds[index];
		}
		
		results.push_back(foundVal);
	}
	return Rcpp::DataFrame::create(
		Rcpp::Named("Displacee", uniqueIds),
		Rcpp::Named("Displacer", results)
	);
}
