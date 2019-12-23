#include "../../../Include/Luna/FE/Object.hpp"

using namespace Luna::FE;

/******************************************************************************/
Object::Object()
{
}

/******************************************************************************/
Object::~Object()
{
}

/******************************************************************************/
void Object::setParent(std::shared_ptr<Object> parent,
                       std::shared_ptr<Object> child)
{
  /* Check if the child currently has a parent. */
  if(child->fParent)
  {
    /* Find the child in the parent's list of children. */
    auto iter = std::find(child->fParent->fChildren.begin(),
                          child->fParent->fChildren.end(),
                          child);

    /* Check whether it was found in the parent's child list. */
    if(iter != child->fParent->fChildren.end())
    {
      /* If found, then remove it. */
      child->fParent->fChildren.erase(iter);
    }
  }

  /* Set the child's new parent. */
  child->fParent = parent;

  /* Add the child to the new parent's list of children. */
  parent->fChildren.push_back(child);
}

/******************************************************************************/
std::shared_ptr<Object> Object::getParent()
{
  return fParent;
}
