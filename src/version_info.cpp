// Stub source for the library target — provides version symbols.
#include "projectnestor/version.hpp"

namespace projectnestor {

const char* get_version() { return kVersion.data(); }
const char* get_project_name() { return kProjectName.data(); }

}  // namespace projectnestor
