#include "projectnestor/version.hpp"

#include <gtest/gtest.h>

namespace projectnestor::test {

TEST(VersionTest, ProjectNameIsCorrect) {
  EXPECT_EQ(kProjectName, "ProjectNestor");
}

TEST(VersionTest, VersionIsSet) {
  EXPECT_FALSE(kVersion.empty());
}

} // namespace projectnestor::test
