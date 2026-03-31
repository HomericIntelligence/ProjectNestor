#include "projectnestor/version.hpp"

#include <gtest/gtest.h>

namespace projectnestor::test {

TEST(VersionTest, ProjectNameIsCorrect) { EXPECT_EQ(kProjectName, "ProjectNestor"); }

TEST(VersionTest, VersionIsSet) { EXPECT_FALSE(kVersion.empty()); }

TEST(VersionTest, GetVersionReturnsVersion) { EXPECT_STREQ(get_version(), kVersion.data()); }

TEST(VersionTest, GetProjectNameReturnsName) {
  EXPECT_STREQ(get_project_name(), kProjectName.data());
}

}  // namespace projectnestor::test
