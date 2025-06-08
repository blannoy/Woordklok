import React from "react";
import { useTranslation } from "react-i18next";

const LanguageSwitcher = () => {
  const { i18n } = useTranslation();

  const changeLanguage = (lng) => {
    i18n.changeLanguage(lng);
  };

return (
    <div className="language-switcher">
        <select onChange={(e) => changeLanguage(e.target.value)}>
            <option value="nl">Nederlands</option>
            <option value="en">English</option>
        </select>
    </div>
);
};

export default LanguageSwitcher;