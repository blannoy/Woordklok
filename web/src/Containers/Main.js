import React,{useReducer,useEffect} from "react";
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


export default function Main() {


    return (
        <HashRouter>
        <div>
          <h1>Woordklok</h1>
          <ul className="header">
            <li><NavLink to="/">Home</NavLink></li>
            <li><NavLink to="/colors">Colors</NavLink></li>
            <li><NavLink to="/brightness">Brightness</NavLink></li>
            <li><NavLink to="/config">Config</NavLink></li>
          </ul>
          <div className="content">
            <Routes>
            <Route path="/" element={<Home/>}/>
            <Route path="/colors" element={<LEDColors/>}/>
            <Route path="/brightness" element={<Brightness/>}/>
            <Route path="/config" element={<ShowConfig/>}/>
            </Routes>
          </div>
        </div>
        </HashRouter>
    );
  }