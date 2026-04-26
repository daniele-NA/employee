#ifndef JSONMANAGER_JSONPARAMFEATURES_H
#define JSONMANAGER_JSONPARAMFEATURES_H

#include <string>

/**
 * Centralized JSON field names and the expected parameter count.
 * Renaming a field in the on-disk schema is a one-line change here —
 * no other source file should hard-code these strings.
 */
static const int           PARAM_NUMBER     = 5;
static const std::string   PARAM_NAME       = "name";
static const std::string   PARAM_SURNAME    = "surname";
static const std::string   PARAM_TAXCODE    = "tax_code";
static const std::string   PARAM_AGE        = "age";
static const std::string   PARAM_ASSIGNMENT = "assignment";

#endif // JSONMANAGER_JSONPARAMFEATURES_H
