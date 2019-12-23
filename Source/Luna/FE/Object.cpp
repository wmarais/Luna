#include "../../../Include/Luna/FE/Object.hpp"

using namespace Luna::FE;

/******************************************************************************/
Object::Object(Object * parent) : fParent(parent)
{
}

/******************************************************************************/
Object::~Object()
{
  /* Set the parent node of all the children to NULL. */
  for(auto & child : fChildren)
  {
    child->fParent = nullptr;
  }
}

/******************************************************************************/
void Object::addChild(std::shared_ptr<Object> child)
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

  /* Search for this child in the current list of children. */
  auto iter = std::find(fChildren.begin(), fChildren.end(), child);

  /* Add the child if not allready in the list of children. */
  if(iter == fChildren.end())
    fChildren.push_back(child);

  /* Set the parent of the child. */
  child->fParent = this;
}

/******************************************************************************/
Object * Object::getParent()
{
  return fParent;
}

/******************************************************************************/
void Object::render(Renderer & renderer, const Rect & screenArea)
{
  /* Calculate the area of this object that should be repainted. */
  auto area = screenArea.intersect(fScreenRect);

  /* There is nothing more to render. */
  if(!area)
    return;

  /* Repaint the object. */
  repaint(renderer, screenArea);

  /* Repaint all the children. */
  for(auto & child : fChildren)
  {
    child->render(renderer, area.value());
  }
}
