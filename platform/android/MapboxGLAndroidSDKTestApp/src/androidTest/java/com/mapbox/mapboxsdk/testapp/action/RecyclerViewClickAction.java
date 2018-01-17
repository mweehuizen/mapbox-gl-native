package com.mapbox.mapboxsdk.testapp.action;

import android.support.test.espresso.UiController;
import android.support.test.espresso.ViewAction;
import android.view.View;

import org.hamcrest.Matcher;

import static android.support.test.espresso.matcher.ViewMatchers.isDisplayed;

public class RecyclerViewClickAction implements ViewAction {

  private int id;

  public RecyclerViewClickAction(int id) {
    this.id = id;
  }

  @Override
  public Matcher<View> getConstraints() {
    return isDisplayed();
  }

  @Override
  public String getDescription() {
    return "Click on a child view with specified id.";
  }

  @Override
  public void perform(UiController uiController, View view) {
    view.findViewById(id).performClick();
  }
}
