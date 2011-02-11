#include "Common.h"
#include "CustomSlider.h"
#include "FilterControls.h"
#include "FilterValue.h"

class FilterParamSliderGroupModel : public SliderGroup::Model
{
public:
  FilterParamSliderGroupModel (ListenerList<FilterListener>& listeners,
                               Dsp::Filter* filter,
                               int paramIndex)
    : m_listeners (listeners)
    , m_filter (filter)
    , m_paramIndex (paramIndex)
    , m_paramInfo (m_filter->getParameterInfo (paramIndex))
  {
  }

  ~FilterParamSliderGroupModel()
  {
  }

  const String getName () const
  {
    return m_paramInfo.szLabel;
  }

  double getControlValue () const
  {
    return m_filter->getParameters()[m_paramIndex];
  }

  void setControlValue (double controlValue)
  {
    m_filter->setParam (m_paramIndex, controlValue);
    m_listeners.call (&FilterListener::onFilterParameters);
  }

  const String getNativeValueAsText () const
  {
    double nativeValue = 0.5;
    return String (nativeValue, 3);
  }

  void setNativeValue (double nativeValue)
  {
  }

private:
  ListenerList<FilterListener>& m_listeners;
  Dsp::Filter* m_filter;
  int m_paramIndex;
  const Dsp::ParameterInfo m_paramInfo;
};

//------------------------------------------------------------------------------

FilterControls::FilterControls (ListenerList<FilterListener>& listeners)
  : m_listeners (listeners)
{
}

FilterControls::~FilterControls ()
{
  clear ();
}

void FilterControls::paint (Graphics& g)
{
  const Rectangle<int> bounds = getLocalBounds();

  g.setColour (Colour::fromRGBA (0, 0, 0, 64));
  g.fillRect (bounds.reduced (1, 1));

  g.setColour (Colour::fromRGBA (0, 0, 0, 128));
  g.drawRect (bounds, 1);
}

// Use introspection to build the list of controls
void FilterControls::onFilterChanged (Dsp::Filter* newFilter)
{
  m_filter = newFilter;

  clear ();

  const Rectangle<int> bounds = getLocalBounds();

  const int w = 70;
  const int ygap = 0;
  const int h = bounds.getHeight() - 2 * ygap;
  const int y = bounds.getY() + ygap;

  int x = bounds.getX() + 2;
  for (int i = 0; i < m_filter->getNumParameters(); ++i)
  {
    const Dsp::ParameterInfo& info = m_filter->getParameterInfo(i);

    Item item;
    item.group = new SliderGroup (new FilterParamSliderGroupModel (
      m_listeners, m_filter, i));
    item.group->setBounds (x, y, w, h);
    addAndMakeVisible (item.group);

    m_items.add (item);

    x += w;
  }
}

void FilterControls::clear ()
{
  for (int i = 0; i < m_items.size(); ++i)
  {
    delete m_items[i].group;
  }

  m_items.clear ();
}