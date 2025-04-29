import React, { useEffect, useState } from "react";
import { useGetStatusQuery } from "../Components/ClockAPI";


function Home() {
  const { data: statusData, error: statusError, isLoading: statusIsLoading } = useGetStatusQuery();

  const [clock, setClock] = useState(null);
  const [version, setVersion] = useState(null);

  useEffect(() => {
    if (statusData !== undefined) {
      setClock(statusData.currentTime);
      setVersion(statusData.version);
    }
  }, [statusData]);

      return (
        <div>
          <h2>Huidige status</h2>
          <div className="section">
            <div className="labelCell">
              <label>Klok </label>
            </div>
            <div className="smallBodyCell">{clock}</div>
          </div>
          <div className="section">
            <div className="labelCell">
              <label>Versie </label>
            </div>
            <div className="smallBodyCell">{version}</div>
          </div>
          </div>
      );
    }

    export default React.memo(Home);