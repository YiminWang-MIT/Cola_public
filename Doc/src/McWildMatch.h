///////////////////////////////////////////////////////////////////////
//
// $Id: McWildMatch.h 2216 2008-06-13 21:13:47Z distler $
//
///////////////////////////////////////////////////////////////////////
#ifndef MC_WILD_MATCH_H
#define MC_WILD_MATCH_H

/// Matches string against pattern containing wildcards '?' and '*'.
/** This routine tests a string against a wild card pattern. The wild 
    characters are '*' and '?'. '*' matches an arbitrary sequence of
    characters while '?' matches any single character. Returns 1 if the
    match is successful, 0 otherwise. */
extern int mcWildMatch(const char* str, const char* pattern);

#endif
