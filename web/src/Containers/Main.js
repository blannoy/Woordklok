import {
    Route,
    NavLink,
    HashRouter,
    Routes
  } from "react-router-dom";
import Home from "../Views/Home";
import LEDColors from "../Views/LEDColors";
import Brightness from "../Views/Brightness";
import ShowConfig from "../Views/ShowConfig";
import Admin from "../Views/Admin";
import Sensors from "../Views/Sensors";
import LanguageSwitcher from "../Components/LanguageSwitcher";
import { useTranslation } from "react-i18next"; // Add this import

export default function Main() {
    const { t } = useTranslation(); // Add this line

    return (
        <HashRouter>
        <div>
          <h1>{t("woordklokTitle")}</h1>
          <LanguageSwitcher />
          <ul className="header">
            <li><NavLink to="/">{t("nav.home")}</NavLink></li>
            <li><NavLink to="/colors">{t("nav.colors")}</NavLink></li>
            <li><NavLink to="/brightness">{t("nav.brightness")}</NavLink></li>
            <li><NavLink to="/admin">{t("nav.admin")}</NavLink></li>
            <li><NavLink to="/config">{t("nav.config")}</NavLink></li>
            <li><NavLink to="/sensor">{t("nav.sensors")}</NavLink></li>
          </ul>
          <div className="content">
            <Routes>
            <Route path="/" element={<Home/>}/>
            <Route path="/colors" element={<LEDColors/>}/>
            <Route path="/brightness" element={<Brightness/>}/>
            <Route path="/admin" element={<Admin/>}/>
            <Route path="/config" element={<ShowConfig/>}/>
            <Route path="/sensor" element={<Sensors/>}/>
            </Routes>
          </div>
        </div>
        </HashRouter>
    );
  }
